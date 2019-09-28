var net = require('net');

// This function create and return a net.Socket object to represent TCP client.
function getConn(connName){

    const option = {
        host:'127.0.0.1',
        port: 8080
    }

    // Create TCP client.
    const client = net.createConnection(option, function () {
        console.log('Connection name : ' + connName);
        console.log('Connection local address : ' + client.localAddress + ":" + client.localPort);
        console.log('Connection remote address : ' + client.remoteAddress + ":" + client.remotePort);
    });

    client.setTimeout(1000);
    client.setEncoding('utf8');

    // When receive server send back data.
    client.on('data', function (data) {
        console.log('Server return data : ' + data);
    });

    client.on('end',function () {
        console.log('Client socket disconnect. ');
    });

    client.on('timeout', function () {
        console.log('Client connection timeout. ');
    });

    client.on('error', function (err) {
        console.error(JSON.stringify(err));
    });

    return client;
}

// Create a java client socket.
const client_1 = getConn('Thomas');

// Create node client socket.
const client_2 = getConn('Jolien');

client_1.write(JSON.stringify({
    name: "Thomas",
    age: 31,
    status: "Flowed",
    city: "Breda",
 }));
client_1.end();

client_2.write(JSON.stringify({
    name: "Jolientje",
    age: 29,
    status: "Lief",
    city: "Breda",
 }));
client_2.end();