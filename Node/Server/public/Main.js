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
        case 'event':
                Notify_Event(data);
            break;
        case 'response':
                Notify_Response(data);
            break;
        case 'error':
                Notify_Error(data);
            break;
        case 'action':
                Notify_Action(data);
            break;
        default:
            console.log(`Unknown server type message: ${data.type}`)            
    }
});

/////////////////////////////////////////////////

function Notify_Error(data)
{
    $.notify({
        // options
        message: `${data.event.device_name}: ${data.event.payload_0}` 
    },{
        // settings
        type: 'danger',
        placement: {
            from: "top",
            align: "center"
        },
        timer: 0,
        animate: {
            enter: 'animated fadeInDown',
            exit: 'animated fadeOut'
        },
    });
}

function Notify_Event(data)
{
    $.notify({
        // options
        message: `${data.event.device_name}: ${data.event.payload_0}` 
    },{
        // settings
        type: 'success',
        placement: {
            from: "top",
            align: "center"
        },
        animate: {
            enter: 'animated fadeInDown',
            exit: 'animated fadeOut'
        },
    });
}

function Notify_Response(data)
{
    $.notify({
        // options
        message: `${data.event.device_name}: ${data.event.payload_0}` 
    },{
        // settings
        type: 'info',
        placement: {
            from: "top",
            align: "center"
        },
        timer: 1500,
        animate: {
            enter: 'animated fadeInDown',
            exit: 'animated fadeOut'
        },
    });
}

function Notify_Action(data)
{
    $.notify({
        // options
        message: `${data.event.device_name}: ${data.event.payload_0}` 
    },{
        // settings
        type: 'warning',
        placement: {
            from: "top",
            align: "center"
        },
        timer: 1500,
        animate: {
            enter: 'animated fadeInDown',
            exit: 'animated fadeOut'
        },
    });
}

/////////////////////////////////////////////////

function ExecuteQuickAction()
{ 
    let device_name = document.getElementById(`quick_action_device`).value; 
    let action_name = document.getElementById(`quick_action`).value; 
    let parameter   = document.getElementById(`quick_parameter`).value;

    console.log(`Executing Quick Action: ${device_name} : ${action_name}`);

    socket.emit('server', {
        type: 'request', 
        command: 'action', 
        deviceName: device_name,
        actionName: action_name,
        actionParameter: parameter
    });   
}

function SimulateQuickEvent()
{
    let device_name = document.getElementById(`quick_event_device`).value; 
    let event_name = document.getElementById(`quick_event`).value;        

    console.log(`Simulate event: ${event_name}`);    

    socket.emit('server', {
        type: 'request', 
        command: 'event', 
        deviceName: device_name,
        eventName: event_name
    });   
}

/////////////////////////////////////////////////

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