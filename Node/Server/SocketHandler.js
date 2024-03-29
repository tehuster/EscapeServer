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
                     console.log(`Received request from WebClient: ${data.deviceName} @ ${data.actionName}`)
                     this.emit('request', data)
                     break;
                  }
                  case 'hints' :
                  {
                     console.log(`Received hint from WebClient: ${data.hint} for ${data.room}`)
                     this.emit('hints', data)
                     break;
                  }
                  case 'devices' :
                  {
                     console.log(`Received device command WebClient: ${data.command}`)
                     this.emit('devices', data)
                     break;
                  }
                  case 'triggers' :
                  {
                     console.log(`Received trigger command WebClient: ${data.command}`)
                     this.emit('triggers', data)
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