const socket = io();

socket.io.on('connect_error', function(err) {
    openModal("error", "Server Disconnected");
    console.log('Error connecting to server');
});

socket.emit('server', {type: 'system', msg: `alive`});

socket.on('webclient', function (data) {  
    console.log(`Server: ${data}`);
    switch (data.type) { 
        case 'refresh':
            location.reload();
            break;       
    }
});