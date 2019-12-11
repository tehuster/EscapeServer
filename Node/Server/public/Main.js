const socket = io();

socket.io.on('connect_error', function (err) {
    openModal("error", "Server Disconnected");
    console.log('Error connecting to server');
});

socket.emit('server', { type: 'system', msg: `alive` });

socket.on('webclient', function (data) {
    console.log(`Server: ${data}`);
    switch (data.type) {
        case 'refresh':
            location.reload();
            break;
    }
});

var devices;
var triggers;

fetch('./api')
    .then(
        function (response) {
            if (response.status !== 200) {
                console.log('Looks like there was a problem. Status Code: ' +
                    response.status);
                return;
            }
            // Examine the text in the response
            response.json().then(function (data) {
                devices = data.devices;
                triggers = data.triggers;
            });
        }
    )
    .catch(function (err) {
        console.log('Fetch Error :-S', err);
    });

