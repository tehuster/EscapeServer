console.log('MidiHandler init...');

// Enable WebMidi.js
WebMidi.enable(function (err) {

    if (err) {
        console.log('WebMidi could not be enabled.', err);
    }

    // Viewing available inputs and outputs
    //console.log(WebMidi.inputs);
    //console.log(WebMidi.outputs);

    // Display the current time
    //console.log(WebMidi.time);

    // Retrieve an input by name, id or index
    const input = WebMidi.getInputByName('LPD8');

    // Listen for a 'note on' message on all channels
    input.addListener('noteon', 'all',
        function (value) {
            console.log('Received \'noteon\' message (' + value.note.name + value.note.octave + ').');

            let note = value.note.name + value.note.octave;

            switch (note) {
                case 'E2':
                    socket.emit('server', {type: 'midi', msg: '0'});
                    break;
                case 'F2':
                    socket.emit('server', {type: 'midi', msg: '1'});
                    break;
                case 'C2':
                    socket.emit('server', { type: 'midi', msg: '4' });
                    break;
                case 'C#2':
                    socket.emit('server', { type: 'midi', msg: '5' });
                    break;
                case 'D2':
                    socket.emit('server', { type: 'midi', msg: '7' });
                    break;
                case 'D#2':
                    socket.emit('server', { type: 'midi', msg: '8' });
                    break;
                case 'G2':
                    socket.emit('server', { type: 'midi', msg: '3' });
                    break;
                case 'F2':
                    socket.emit('server', { type: 'midi', msg: '7' });
                    break;
                case 'F#2':
                    socket.emit('server', { type: 'midi', msg: '2' });
                    break;
            }
        }
    );

    // Listen to control change message on all channels
    input.addListener('controlchange', 'all',
        function (value) {
            console.log('Received \'controlchange\' message.', value);
        }
    );

});