const WebClient = require('./WebClient')
const webClient = new WebClient()

const RequestHandler = require('./RequestHandler')
const requestHandler = new RequestHandler()

requestHandler.addRequest(101, 'data')

const TCP = require('./TCP')
const tcp = new TCP(8080, '192.168.1.249')

const SocketHandler = require('./SocketHandler')
const socketHandler = new SocketHandler(webClient.server)

///////////////////////// SOCKET-EVENTS 

socketHandler.on('request', (data) => {
   console.log('Handle request event!');   
   requestHandler.addRequest(data.requestAddress, data.requestType);
   console.log(requestHandler.requests);   
})

///////////////////////// TCP-EVENTS   

tcp.on('tcp', (data) => {
   console.log('Handling TCP event...' + data) 
   let deviceAddress = JSON.parse(data).status.address;
   let request = requestHandler.checkForRequest(deviceAddress);
   tcp.setRequest(request);
});

///////////////////////// MIDI-EVENTS

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

