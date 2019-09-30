const EventEmitter = require('events')

class SocketHandler extends EventEmitter
{
    constructor(server)
    {
        super()
        console.log('SocketHandler init')   
        this.io = require('socket.io')(server)
        this.configureSocket()
    }

    configureSocket()
    {
        this.io.on('connection', (socket) => {
            socket.on('server', (data) => {
               //console.log(`WebClient: ${data}`)
               switch (data.type) {
                  case 'system' :
                  {
                     console.log(`Received system message from WebClient: ${data.msg}`);
                     this.emit('system', data)
                     break;
                  }
                  case 'request' :
                  {
                     console.log(`Received request from WebClient: ${data.requestType} @ ${data.requestAddress}`)
                     this.emit('request', data)
                     break;
                  }
                  case 'midi' :
                  {
                     console.log(`Received midi from WebClient: ${data.msg}`)
                     this.emit('midi', data.msg)
                     break;
                  }
                  default :
                  {
                     console.log(`Unknown message type received from WebClient: ${data.type}`);            
                     break;
                  }      
               }
            })
         })
    }
}

module.exports = SocketHandler;