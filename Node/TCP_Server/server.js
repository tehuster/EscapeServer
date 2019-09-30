const WebClient = require('./WebClient')
const webClient = new WebClient()

const TCP = require('./TCP')
const tcp = new TCP(8080, '192.168.1.249')

const RequestHandler = require('./RequestHandler')
const requestHandler = new RequestHandler()

const SocketHandler = require('./SocketHandler')
const socketHandler = new SocketHandler(webClient.server)

socketHandler.on('request', (data) => {
   console.log('Handle request event!');   
   requestHandler.addRequest(data.requestAddress, data.requestType);
   console.log(requestHandler.requests);   
})

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

