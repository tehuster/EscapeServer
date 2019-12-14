
function AddTrigger()
{
    let name = document.getElementById('name').value;
    let iDevice = document.getElementById('iDevice').value;
    let event = document.getElementById('event').value;
    let oDevice = document.getElementById('oDevice').value;
    let action = document.getElementById('action').value;
    let parameter = document.getElementById('parameter').value;
    let delay_time = document.getElementById('delay_time').value;

    console.log(name, iDevice, event, oDevice, action);

    socket.emit('server', {
        type: 'triggers', 
        command: 'add',          
        name: name, 
        iDevice: iDevice,
        event: event, 
        oDevice: oDevice, 
        action: action, 
        parameter: parameter,
        delay_time: delay_time
    });    
}

function RemoveTrigger(id)
{
    console.log("Removing trigger: " + id);

    socket.emit('server', {
        type: 'triggers', 
        command: 'remove',          
        id: id
    });    
}



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