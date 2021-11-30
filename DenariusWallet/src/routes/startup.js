import("../startup.css");

function startupPage() {
	return (
		<div className="App">
			<header className="App-header" style={{backgroundColor:"#66023c", color:"#d19b2f"}}>
				<h1>Solidius Wallet</h1>
				<input id="username" placeholder="Username" required />
				<input id="password" placeholder="Password" required />
			</header>
		</div>
	);
}

export default startupPage;