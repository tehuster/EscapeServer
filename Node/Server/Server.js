//const Database = require('./Database')
const Hints = () => { };//new Database.Hints() Sequelize this.

//THIS COULD AND SHOULD BE CLEANER...
const Sequelize = require('sequelize')
const sqlite = require('./database/sqlite')
const Devices = require('./database/models/devices')(Sequelize, sqlite)
const Actions = require('./database/models/actions')(Sequelize, sqlite)
const Configs = require('./database/models/configs')(Sequelize, sqlite)
const Triggers = require('./database/models/triggers')(Sequelize, sqlite)
const Parameters = require('./database/models/parameters')(Sequelize, sqlite)
const Events = require('./database/models/events')(Sequelize, sqlite)
const Notifications = require('./database/models/notifications')(Sequelize, sqlite)

const Models = {
   Devices,
   Actions,
   Configs,
   Triggers,
   Parameters,
   Events,
   Notifications
}

Devices.associate(Models)
Actions.associate(Models)
Configs.associate(Models)
Triggers.associate(Models)
Parameters.associate(Models)
Events.associate(Models)

// Notifications.destroy({
//    where: {},
//    truncate: true
//  })

// const ResponseHandler = require('./ResponseHandler')
// const responseHandler = new ResponseHandler()
// responseHandler.loadDevices(Devices.findAll())

///////////////////////// MQTT
const MQTT = require('./MQTT')
const mqtt = new MQTT()

mqtt.triggers = Triggers;

///////////////////////// WEBCLIENT
const WebClient = require('./WebClient')
const webClient = new WebClient(Devices, Hints, Triggers, Notifications)

///////////////////////// SOCKET
const SocketHandler = require('./SocketHandler')
const socketHandler = new SocketHandler(webClient.server)

socketHandler.on('request', (data) => {
   switch (data.command) {
      case 'manual':
         mqtt.sendMessage(data.deviceName, data.requestType, data.actionName, data.actionParameter);
         break;
      case 'action':
         mqtt.sendMessage(data.deviceName, 'Action', data.actionName, data.actionParameter);
         break;
      case 'event':
         mqtt.sendMessage(data.deviceName, 'Event', data.eventName, null);
         break;
      default:
         console.log('Unknown request command received...')
         break;
   }
})

///////////////////////// MQTT
mqtt.on('notification', (data) => {
   let notification_color;
   let saveNotification = false;
   switch (data.topic_type) {
      case 'Event':
         socketHandler.io.emit('webclient', { type: 'event', event: data }) 
         notification_color = 'success';            
         saveNotification = true;           
         break;
      case 'Error':
         socketHandler.io.emit('webclient', { type: 'error', event: data })
         notification_color = 'danger';         
         saveNotification = true;
         break;
      case 'Response':
         socketHandler.io.emit('webclient', { type: 'response', event: data })
         notification_color = 'info';
         saveNotification = true;
         break;
      case 'Action':
         socketHandler.io.emit('webclient', { type: 'action', event: data })
         notification_color = 'warning';
         saveNotification = true;
         break;
      default:
         console.log(`Unknown topic type: ${data.topic_type}`)                   
   }
   if(saveNotification)
   {
      Notifications.create({
         topic_name: data.device_name,
         topic_type: data.topic_type,
         payload_0:  data.payload_0,
         payload_1:  data.payload_1,
         notification_color: notification_color
      }).then(() => {
         //responseHandler.loadDevices(Devices.findAll())
         //socketHandler.io.emit('webclient', { type: 'refresh' })
      });
   }   
})

mqtt.on('error', (data) => {
   
})

///////////////////////// DEVICES
socketHandler.on('devices', (data) => {
   switch (data.command) {
      case 'add':
         Devices.create({
            device_room: data.room,
            device_name: data.name,
            device_description: data.description
         }).then(() => {
            //responseHandler.loadDevices(Devices.findAll())
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'remove':
         Devices.destroy({
            where: { id: data.id }
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'addAction':
         Actions.create({
            DeviceId: data.device_id,
            action_name: data.action_name,
            action_parameter_type: data.action_parameter,
            action_public: data.action_public
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'removeAction':
         Actions.destroy({
            where: { id: data.id }
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'addConfig':
         Configs.create({
            DeviceId: data.device_id,
            config_name: data.config_name,
            config_type: 'string',
            public: data.config_public            
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'addEvent':
         Events.create({
            DeviceId: data.device_id,
            event_name: data.event_name       
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'removeEvent':
         Events.destroy({
            where: { id: data.id }
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      default:
         console.log('Unknown device command received...')
         break;
   }
})

///////////////////////// Triggers
socketHandler.on('triggers', (data) => {
   switch (data.command) {
      case 'add':
         Triggers.create({
            trigger_name: data.name,
            iDevice: data.iDevice,
            event: data.event,
            oDevice: data.oDevice,
            action: data.action, 
            parameter: data.parameter,
            delay_time: data.delay_time
         }).then(() => {
            //responseHandler.loadDevices(Devices.findAll())
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'remove':
         Triggers.destroy({
            where: { id: data.id }
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      default:
         console.log('Unknown trigger command received...')
         break;
   }
})

///////////////////////// HINTS
socketHandler.on('hints', (data) => {
   switch (data.command) {
      case 'add':
         hints.add(data.room, data.hint)
         break;
      default:
         console.log('Unknown hint command received...')
         break;
   }
})

///////////////////////// MIDI
socketHandler.on('midi', (data) => {
   if (data == 0) {
      // requestHandler.clearAllRequests()
   } else {
      // requestHandler.addRequest(data * 10, 'data')
      // console.log('Added request to queue. Current queue: ')
      // console.log(requestHandler.requests)
   }
})

///////////////////////
// setInterval(PollDeviceResponses, 1000);

// function PollDeviceResponses() {
//    let date = new Date;
//    let currentTime = date.getTime()

//    responseHandler.responses.forEach(function (response) {  
//       let lastTimeSinceRespone = (currentTime - response.time)/1000;
//       if(lastTimeSinceRespone > 5)
//       {
//          console.log(`Error: Last response from ${response.name} was ${lastTimeSinceRespone} seconds ago.`)
//       }          
//    }) 
// }

