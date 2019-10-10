const net = require('net')
const EventEmitter = require('events')

class TCP extends EventEmitter
{
    constructor(port = 8080, ip = '192.168.1.249', requestHandler) {
        super() 
        this.port = port;
        this.ip = ip;
        this.server = net.createServer();     
        this.requestHandler = requestHandler;

        this.server.listen(this.port, this.host, function () {
            console.log(`TCP_Server started on port ${port} at ${ip}`)
        })
        this.setupServer()
    }

    setRequest(request)
    {
        this.request = request;
    }

    setupServer() {
        let tcp = this;
        // emitted when new client connects
        this.server.on('connection', function (socket) {

            this.getConnections(function (error, count) {
                //console.log('Number of concurrent connections to the server : ' + count)
            })

            socket.on('data', function (data) {                   
                tcp.requestHandler.handleResponse(data.toString()) 
                tcp.requestHandler.checkForRequest(10)
                    .then(async (result) => {                                                                 
                        socket.write(result)                     
                    })           
            })

            socket.on('error', function (error) {
                console.log('Error : ' + error)
            })

            socket.on('timeout', function () {
                console.log('Socket timed out !')
                socket.end('Timed out!')
                // can call socket.destroy() here too.
            })

            socket.on('end', function (data) {
                //console.log('Socket ended from other end!')
                //console.log('End data : ' + data)
            })

            socket.on('close', function (error) {
                if (error) {
                    console.log('Socket was closed coz of transmission error')
                }
            })
        })

        this.server.on('close', function () {
            console.log('Server closed !')
        })
    }
}

module.exports = TCP;

