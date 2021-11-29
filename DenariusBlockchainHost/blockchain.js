const { spawn } = require('child_process');
const { workerData, parentPort } = require('worker_threads')

const blockchain = spawn('../Debug/Denarius.exe', [''])

blockchain.stdout.on("data", (data) => {
    parentPort.postMessage(`Blockchain stdout: ${data}`);
});

blockchain.stderr.on("err", (data) => {
    parentPort.postMessage(`blockchain stderr: ${data}`);
});
