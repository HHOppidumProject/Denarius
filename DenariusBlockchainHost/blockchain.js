const { execFile } = require('child_process');
const { workerData, parentPort } = require('worker_threads')

    const blockchain = execFile('../Debug/Denarius.exe', [''], (error, stdout, stderr) => {
        if (error) {
            throw error;
        }
    });

    blockchain.stdout.on("data", (data) => {
        parentPort.postMessage(`Blockchain stdout: ${data}`);
    });

    blockchain.stderr.on("err", (data) => {
        parentPort.postMessage(`blockchain stderr: ${data}`);
    });
