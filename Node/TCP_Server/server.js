const RequestHandler = require('./RequestHandler')
const requestHandler = new RequestHandler()
requestHandler.addRequest(101, 'data')

const Database = require('./Database')

///////////////////////// WEBCLIENT
const WebClient = require('./WebClient')
const webClient = new WebClient()


///////////////////////// SOCKET
const SocketHandler = require('./SocketHandler')
const socketHandler = new SocketHandler(webClient.server)

socketHandler.on('request', (data) => {
   console.log('Handle request event!');   
   requestHandler.addRequest(data.requestAddress, data.requestType);
   console.log(requestHandler.requests);   
})

///////////////////////// TCP  
const TCP = require('./TCP')
const tcp = new TCP(8080, '192.168.1.249')

tcp.on('tcp', (data) => {
   console.log('Handling TCP event...' + data) 
   let deviceAddress = JSON.parse(data).status.address;
   let request = requestHandler.checkForRequest(deviceAddress);
   tcp.setRequest(request);
});

///////////////////////// DEVICES
const devices = new Database.Devices()
// devices.add(10, 'room', 'name', 'description', 'config', 'actions')
// devices.get().then(function(result) { console.log(result) }) 
// devices.update(2, 10, 'name', 'description', 'config', 'actions')
// devices.remove(1)

///////////////////////// HINTS
const hints = new Database.Hints()
// hints.add('room', 'hint')
// hints.get().then(function(result) { console.log(result) }) 
// hints.update(2, 'room', 'hint')
// hints.remove(1)

///////////////////////// MIDI
socketHandler.on('midi', (data) => {
   if(data == 0)
   {
      requestHandler.clearAllRequests()
   }else
   {      
      requestHandler.addRequest(data * 10, 'data')
      console.log('Added request to queue. Current queue: ')
      console.log(requestHandler.requests);   
   }  
})

