const RequestHandler = require('./RequestHandler')
const requestHandler = new RequestHandler()

const ResponseHandler = require('./ResponseHandler')
const responseHandler = new ResponseHandler()

const Database = require('./Database')
const devices = new Database.Devices()
const hints = new Database.Hints()

responseHandler.loadDevices(devices.get())

///////////////////////// WEBCLIENT
const WebClient = require('./WebClient')
const webClient = new WebClient(devices, hints)

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
         devices.add(            
            data.room,
            data.name,
            data.description
         ).then(() => {
            responseHandler.loadDevices(devices.get())
            socketHandler.io.emit('webclient', {type:'refresh'})
         });
         break;
      case 'remove':
         devices.remove(data.id).then(() => {
            socketHandler.io.emit('webclient', {type:'refresh'})
         });
         break;
      case 'addAction':
         devices.addAction(data.action_name, data.action_parameter, data.device_id).then(() => {
            socketHandler.io.emit('webclient', {type:'refresh'})
         });
         break;
      case 'addConfig':
         devices.addConfig(data.config_name, data.device_id).then(() => {
            socketHandler.io.emit('webclient', {type:'refresh'})
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

