import { Link } from "react-router-dom";

/*
 * 
 */

function navBar() {
    return (
        <nav class="sidebar">
            <img src="/assets/solidus.png" />
            <div>
                <Link to="/"><i class="fas fa-home"></i> Home</Link>
                <Link to="/about"><i class="fas fa-wallet"></i> Wallet</Link>
                <Link to="/users"><i class="fas fa-history"></i> History</Link>
            </div>
        </nav>
    )
}

export default navBar;