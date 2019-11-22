//const Database = require('./Database')
const Hints = () => {};//new Database.Hints() Sequelize this.

//THIS COULD AND SHOULD BE CLEANER...
const Sequelize = require('sequelize')
const sqlite = require('./database/sqlite')
const Devices = require('./database/models/devices')(Sequelize, sqlite)
const Actions = require('./database/models/actions')(Sequelize, sqlite)
const Configs = require('./database/models/configs')(Sequelize, sqlite)
const Triggers = require('./database/models/triggers')(Sequelize, sqlite)
const Models = {
   Devices,
   Actions, 
   Configs,
   Triggers
}
Devices.associate(Models)
Actions.associate(Models)
Configs.associate(Models)
Triggers.associate(Models)

// const ResponseHandler = require('./ResponseHandler')
// const responseHandler = new ResponseHandler()
// responseHandler.loadDevices(Devices.findAll())

///////////////////////// MQTT
const MQTT = require('./MQTT')
const mqtt = new MQTT()
 
///////////////////////// WEBCLIENT
const WebClient = require('./WebClient')
const webClient = new WebClient(Devices, Hints, Triggers)

///////////////////////// SOCKET
const SocketHandler = require('./SocketHandler')
const socketHandler = new SocketHandler(webClient.server)

socketHandler.on('request', (data) => {
   mqtt.sendMessage(data.requestName, data.requestType, data.actionName, data.actionParameter);
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
            responseHandler.loadDevices(Devices.findAll())
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
         console.log(data.device_id);
         
         Actions.create({
            DeviceId: data.device_id,
            action_name: data.action_name,
            action_parameter_type: data.action_parameter
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'addConfig':
         Configs.create({
            DeviceId: data.device_id,
            config_name: data.config_name,
            config_type: 'string'
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      default:
         console.log('Unknown hint command received...')
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
            iAction: data.iAction, 
            oDevice: data.oDevice, 
            oAction: data.oAction
         }).then(() => {
            //responseHandler.loadDevices(Devices.findAll())
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
      requestHandler.clearAllRequests()
   } else {
      requestHandler.addRequest(data * 10, 'data')
      console.log('Added request to queue. Current queue: ')
      console.log(requestHandler.requests)
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

