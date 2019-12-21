// let totalProgress = 0;
let namesProgress = 0;
let knockerProgress = 0;
let worldProgress = 0;

socket.on('webclient', function (data) {
    switch (data.type) {
        case 'progress':
            UpdateProgress(data);
            break;       
    }
});

function NewGame()
{
    ResetUI(); 
    // ResetPuzzles();
}

function ResetUI()
{
    console.log('Resetting..');    
    // totalProgress = 0;
    namesProgress = 0;
    knockerProgress = 0;
    worldProgress = 0;
    // setTotal(totalProgress)
    setNames(namesProgress)
    setKnocker(knockerProgress)
    setWorld(worldProgress)

    for (let i = 0; i < 7; i++) {
        setIcon('names', i, 'waiting')   
    }
}

function UpdateProgress(data)
{
    console.log('Updating Progress');    
    console.log(data.event.device_name);  

    switch (data.event.device_name) {
        case 'Names':
            namesProgress += data.event.payload_1 * 1
            setNames();
            setIcon('names', data.event.payload_0, 'completed');
            break;
        case 'Knocker':
            kockerProgress += data.event.payload_1 * 1
            setKnocker();    
            setIcon('names', data.event.payload_0, 'completed');
            break;    
        case 'World':
            worldProgress += data.event.payload_1 * 1
            setWorld()   
            setIcon('names', data.event.payload_0, 'completed');        
            break;
        default:
            break;
    } 
}

///////////////////// DOM

// const totalProgressElement = document.getElementById('progress_total');
const namesProgressElement = document.getElementById('progress_names');
const knockerProgressElement = document.getElementById('progress_knocker');
const worldProgressElement = document.getElementById('progress_world');

// const totalProgressTextElement = document.getElementById('progress_total_text');
const namesProgressTextElement = document.getElementById('progress_names_text');
const knockerProgressTextElement = document.getElementById('progress_knocker_text');
const worldProgressTextElement = document.getElementById('progress_world_text');

// function setTotal()
// {
//     totalProgressElement.setAttribute("style", `width: ${totalProgress}%`);
//     totalProgressTextElement.innerHTML = '%' + totalProgress;
// }

function setNames()
{
    console.log('SettingNames ' + namesProgress);    
    namesProgressElement.setAttribute("style", `width: ${namesProgress}%`);
    namesProgressTextElement.innerHTML = '%' + namesProgress;
}

function setKnocker()
{
    knockerProgressElement.setAttribute("style", `width: ${knockerProgress}%`);
    knockerProgressTextElement.innerHTML = '%' + knockerProgress;
}

function setWorld()
{
    worldProgressElement.setAttribute("style", `width: ${worldProgress}%`);
    worldProgressTextElement.innerHTML   =  '%' + worldProgress;
}

function setIcon(name, index, status)
{
    iconElement = document.getElementById(`${name}_icon_${index}`)
    console.log(`${name}_icon_${index}`);
    
    switch (status) {
        case 'completed':
            iconElement.className = "flaticon2-checkmark kt-font-success"; 
            break;
        case 'waiting':
            iconElement.className = "flaticon-refresh kt-font-brand";
        default:
            break;
    }
}

///////////////////// EXECUTION

function ExecuteAction(device_name, action_name, parameter)  //Add paramaters
{        
    console.log(`Requesting: ${action_name} @ ${device_name} : ${parameter}`)
    socket.emit('server', {
        type: 'request', 
        command: 'action', 
        deviceName: device_name,
        actionName: action_name,
        actionParameter: parameter
    });     
}