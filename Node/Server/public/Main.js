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

function PopulateDropDown(iMenu, oMenu, type)
{   
    let inputMenu = document.getElementById(iMenu);
    let deviceName = inputMenu[inputMenu.selectedIndex].value;    
    let device = devices.find(d => d.device_name == deviceName);
    let outputMenu = document.getElementById(oMenu);
    outputMenu.options.length = 0;

    if(type == "action")
    {
        console.log(device);
        
        device.actions.forEach(action => {
            let option = document.createElement("option");
            option.text = action.action_name;
            outputMenu.append(option)
        });
    }else if(type == "event")
    {        
        device.events.forEach(event => {
            let option = document.createElement("option");
            option.text = event.event_name;
            outputMenu.append(option)
        });
    }
}