importScripts("https://www.gstatic.com/firebasejs/8.10.0/firebase-app.js");
importScripts("https://www.gstatic.com/firebasejs/8.10.0/firebase-messaging.js");
var firebaseConfig = {
    apiKey: "AIzaSyCiBFg77_NsgLK2p0ZQeT_fDWlgUQ2_62g",
    authDomain: "esp32-web-smart-trashcan.firebaseapp.com",
    databaseURL: "https://esp32-web-smart-trashcan-default-rtdb.firebaseio.com",
    projectId: "esp32-web-smart-trashcan",
    storageBucket: "esp32-web-smart-trashcan.appspot.com",
    messagingSenderId: "133964217426",
    appId: "1:133964217426:web:8103088e9792088f79b38b"
};

firebase.initializeApp(firebaseConfig);

const messaging =  firebase.messaging();
messaging.setBackgroundMessageHandler(function(payload){
    const title = 'Test Notification';
    const options = {
        body: payload.data.status
    };
    return self.registration.showNotification(title,options);
});