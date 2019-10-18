const RequestHandler = require('./RequestHandler')
const requestHandler = new RequestHandler()

const ResponseHandler = require('./ResponseHandler')
const responseHandler = new ResponseHandler()

const Database = require('./Database')
const devices = new Database.Devices()
responseHandler.loadDevices(devices.get())
const hints = new Database.Hints()

///////////////////////// WEBCLIENT
const WebClient = require('./WebClient')
const webClient = new WebClient(devices, hints)

///////////////////////// SOCKET
const SocketHandler = require('./SocketHandler')
const socketHandler = new SocketHandler(webClient.server)

socketHandler.on('request', (data) => {
   console.log('Handle request event!');
   requestHandler.addRequest(data.requestAddress, data.requestType);
   console.log(requestHandler.requests);
})

///////////////////////// DEVICES
socketHandler.on('devices', (data) => {
   switch (data.command) {
      case 'add':
         devices.add(
            data.address,
            data.room,
            data.name,
            data.description,
            data.config,
            data.actions
         ).then(() => {
            responseHandler.loadDevices(devices.get())
         });
         break;
      case 'remove':
         devices.remove(data.id)
         break;
      default:
         console.log('Unknown hint command received...')
         break;
   }
})
// devices.add(10, 'room', 'name', 'description', 'config', 'actions')
// devices.get().then(function(result) { console.log(result) }) 
// devices.update(2, 10, 'name', 'description', 'config', 'actions')
// devices.remove(1)

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
// hints.add('room', 'hint')
// hints.get().then(function(result) { console.log(result) }) 
// hints.update(2, 'room', 'hint')
// hints.remove(1)

///////////////////////// TCP  
const TCP = require('./TCP')
const tcp = new TCP(8080, '192.168.1.249', requestHandler, responseHandler)

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

