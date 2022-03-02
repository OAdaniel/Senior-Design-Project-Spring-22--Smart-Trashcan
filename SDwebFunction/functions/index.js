const functions = require("firebase-functions");
const admin = require("firebase-admin");

admin.initializeApp();

//function to prepare for sending message
require('dotenv').config();

//function to just store message received 
exports.inboundSMS = functions.https.onRequest(async (req, res) => {
let params;
    if(Object.keys(req.query).length == 0){
    params = req.body;
}
    else{
    params = req.query;
    }
    await admin.database().ref('/msgq').push(params);
    res.sendStatus(200);
})

//const functions = require('firebase-functions');
//const admin = require('firebase-admin');
const Vonage = require('@vonage/server-sdk');

//admin.initializeApp();

// If you are using .env
const vonage = new Vonage({
  apiKey: process.env.VONAGE_API_KEY,
  apiSecret: process.env.VONAGE_API_SECRET,
});
//

// This function listens for updates to the Firebase Realtime Database
// and sends a message back to the original sender
exports.sendSMS = functions.database
  .ref('/msgq/{pushId}')
  .onCreate(async (message) => {
    const { msisdn, text, to } = message.val();
    const result = await new Promise((resolve, reject) => {
      vonage.message.sendSms('+'+msisdn, to, `You sent the following text: ${text}`, (err, responseData) => {
        if (err) {
          return reject(new Error(err));
        } else {
          if (responseData.messages[0].status === '0') {
            return resolve(
              `Message sent successfully: ${responseData.messages[0]['message-id']}`
            );
          } else {
            return reject(
              new Error(
                `Phone number ${msisdn} Message failed with error: ${responseData.messages[0]['error-text']}`
              )
            );
          }
        }
      });
    });
    return message.ref.parent.child('result').set(result);
  });


//Twilio
  const accountSid = process.env.TWILIO_ACCOUNT_SID;
  const authToken =  process.env.TWILIO_AUTH_TOKEN;

  const client = require('twilio')(accountSid,authToken);

  const twilioNumber = '+19106139872'

  exports.rtdbSendMsg = functions.database
  .ref('/Device1/{pushId}')
  .onCreate(async (currVals) => {
    const { airQuality, depth } = currVals.val();

    if(depth<=10 || airQuality >= 1000){

      
      return admin.database()
      .ref('/msgq/{msgPushId}')
      .once('value')
      .then(snapshot => snapshot.val())
      .then(msgq => {
          //const phoneNumber = '+'+msgq.msisdn;
          const phoneNumber = '+16012148020'

          if ( !validE164(phoneNumber) ) {
              throw new Error('number must be E164 format!')
          }

          const textMessage = {
              body: `The Trashcan needs to be emptied`,
              to: phoneNumber,  // Text to this number
              from: twilioNumber // From a valid Twilio number
          }

          return client.messages.create(textMessage)
      })
      .then(message => console.log(message.sid, 'success'))
      .catch(err => console.log(err))
    }
  });


function validE164(num) {
  return /^\+?[1-9]\d{1,14}$/.test(num)
}
