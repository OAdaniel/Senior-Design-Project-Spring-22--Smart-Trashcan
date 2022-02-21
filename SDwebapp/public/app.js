// Variables to save database current values

// To dispay the real time depth and airquality of on the web app
var randomId;
var airQualityVal;
var depthVal;
var commentsRef = firebase.database().ref('Device1');
commentsRef.on('child_added', (data) => {

  //addCommentElement(postElement, data.key, data.val().text, data.val().author);

  airQualityVal = Object.values(data.val())[0];
  depthVal = Object.values(data.val())[1];
  randomId = data.key;
  //console.log(randomId);
  document.getElementById("airQuality").innerHTML = airQualityVal;
  document.getElementById("depth").innerHTML = depthVal;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});


//https://www.youtube.com/watch?v=BsCBCudx58g
//for the messaging side first create a messagiing obj with all the funstionalities
const messaging = firebase.messaging();
//requestin permission from the user ot show notification. returns a promiss
messaging.requestPermission()
.then(function(){
  console.log('Have permission');
  return messaging.getToken();
})
.then(function(token){
  console.log(token);
})
.catch(function(err){
  console.log('Error Occured.');
})

//messaging.getToken({vapidKey: "BHZaYJA1f2-tfyY0qo_TIc4RyOw_anRsqE79DiSWgE-pzkEgczXUCA0FPl1MQvS8ufTI602EERAcH56P1_FET9Y"}); 

messaging.getToken({ vapidKey: "BHZaYJA1f2-tfyY0qo_TIc4RyOw_anRsqE79DiSWgE-pzkEgczXUCA0FPl1MQvS8ufTI602EERAcH56P1_FET9Y" }).then((currentToken) => {
  if (currentToken) {
    console.log(currentToken);
  } else {
    // Show permission request UI
    console.log('No registration token available. Request permission to generate one.');
    // ...
  }
}).catch((err) => {
  console.log('An error occurred while retrieving token. ', err);
  // ...
});

messaging.onMessage(function(payload){
  console.log('onMessage: ', payload);
  const options = {
    body: payload.data.status
};
return self.registration.showNotification(title,options);
});
