const TCP = require('./TCP')
const tcp = new TCP(8080, '192.168.1.249')

const RequestHandler = require('./RequestHandler')
const requestHandler = new RequestHandler()

requestHandler.addRequest(10, 'data')
requestHandler.addRequest(10, 'actions')
requestHandler.addRequest(20, 'data')
requestHandler.checkForRequest(10)

console.log(requestHandler.requests)