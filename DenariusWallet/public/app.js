// Modules to control application life and create native browser window
const { app, BrowserWindow, protocol } = require('electron')
const path = require('path')
const URL = require('url').URL



function createWindow() {
	const { screen } = require("electron")
	const primaryDisplay = screen.getPrimaryDisplay()
	const { width, height } = primaryDisplay.workAreaSize
	// Create the browser window.
	const mainWindow = new BrowserWindow({
		width: width/1.5,
		height: height/1.5,
		webPreferences: {
			preload: path.join(__dirname, 'preload.js'),
		},
		icon: path.join(__dirname, "icon.ico"),
		menuBarVisible: false,
		title: "Solidius Wallet"
	})

	const appURL = app.isPackaged
		? url.format({
			pathname: path.join(__dirname, "index.html"),
			protocol: "file:",
			slashes: true,
		})
		: "http://localhost:3000";
	mainWindow.setTitle("Denarius Wallet");
	mainWindow.removeMenu();
	mainWindow.loadURL(appURL);

	// Automatically open Chrome's DevTools in development mode.
	if (!app.isPackaged) {
		mainWindow.webContents.openDevTools();
	}
}

function setupLocalFilesNormalizerProxy() {
	protocol.registerHttpProtocol(
		"file",
		(request, callback) => {
			const url = request.url.substr(8);
			callback({ path: path.normalize(`${__dirname}/${url}`) });
		},
		(error) => {
			if (error) console.error("Failed to register protocol");
		}
	);
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
	const { session } = require("electron")

	session.defaultSession.webRequest.onHeadersReceived((details, callback) => {
		callback({
			responseHeaders: {
				...details.responseHeaders,
				'Content-Security-Policy': ['script-src \'self\' \'unsafe-inline\'']
			}
		})
	})

	createWindow()

	setupLocalFilesNormalizerProxy();

	app.on("activate", function () {
		// On macOS it's common to re-create a window in the app when the
		// dock icon is clicked and there are no other windows open.
		if (BrowserWindow.getAllWindows().length === 0) {
			createWindow();
		}
	});
})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
	if (process.platform !== 'darwin') app.quit()
})

const allowedNavigationDestinations = "https://my-electron-app.com";
app.on("web-contents-created", (event, contents) => {
	contents.on("will-navigate", (event, navigationUrl) => {
		const parsedUrl = new URL(navigationUrl);
		if (!allowedNavigationDestinations.includes(parsedUrl.origin)) {
			event.preventDefault();
		}
	});
});

app.on('web-contents-created', (event, contents) => {
	contents.on('will-navigate', (event, navigationUrl) => {
		
		event.preventDefault()
		if (parsedUrl.origin !== 'http://locahost:3000') {
			const parsedUrl = new URL(navigationUrl)
		}
	})
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.