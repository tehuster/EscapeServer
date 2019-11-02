const RequestHandler = require('./RequestHandler')
const requestHandler = new RequestHandler()

const ResponseHandler = require('./ResponseHandler')
const responseHandler = new ResponseHandler()

const Database = require('./Database')
// const devices = new Database.Devices()
const hints = new Database.Hints()

const db = require('./db/Devices')

db.Devices.sync({ force: false }).then(() => { });
db.Actions.sync({ force: false }).then(() => { });
db.Configs.sync({ force: false }).then(() => { });

db.Devices.hasMany(db.Actions)
db.Devices.hasMany(db.Configs)

db.Actions.belongsTo(db.Devices)
db.Configs.belongsTo(db.Devices)

db.Devices.findAll({
   include: [db.Actions, db.Configs]
})
   .then((foundUser) => {
      // Get the User with Company datas included
      console.log("//////////////////////////////////");
      foundUser.forEach(element => {
         console.log(element.dataValues.device_name);
         foundUser[0].dataValues.Actions.forEach(element => {
            console.log("   " + element.dataValues.action_name)
         });

         foundUser[0].dataValues.Configs.forEach(element => {
            console.log("      " + element.dataValues.config_name)
         });
      });
   })
   .catch((err) => {
      console.log("Error while find user : ", err)
   })

responseHandler.loadDevices(db.Devices.findAll())

///////////////////////// WEBCLIENT
const WebClient = require('./WebClient')
const webClient = new WebClient(db.Devices, hints)

///////////////////////// SOCKET
const SocketHandler = require('./SocketHandler')
const socketHandler = new SocketHandler(webClient.server)

socketHandler.on('request', (data) => {
   console.log('Handle request event!');
   requestHandler.addRequest(data.requestName, data.requestType, data.actionName, data.actionParameter);
   console.log(requestHandler.requests);
})

///////////////////////// DEVICES
socketHandler.on('devices', (data) => {
   switch (data.command) {
      case 'add':
         db.Devices.create({
            device_room: data.room,
            device_name: data.name,
            device_description: data.description
         }).then(() => {
            responseHandler.loadDevices(db.Devices.findAll())
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'remove':
         console.log(data.id);

         db.Devices.destroy({
            where: { id: data.id }
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'addAction':
         db.Actions.create({
            DeviceId: data.device_id,
            action_name: data.action_name,
            action_parameter_type: data.action_parameter
         }).then(() => {
            socketHandler.io.emit('webclient', { type: 'refresh' })
         });
         break;
      case 'addConfig':
         db.Configs.create({
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

///////////////////////// HINTS
socketHandler.on('hints', (data) => {
   switch (data.command) {
      case 'add':
         hints.add(data.room, data.hint);
         break;
      default:
         console.log('Unknown hint command received...');
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
      console.log(requestHandler.requests);
   }
})

///////////////////////// TCP  
const TCP = require('./TCP')
const tcp = new TCP(8080, '192.168.1.249', requestHandler, responseHandler)

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

