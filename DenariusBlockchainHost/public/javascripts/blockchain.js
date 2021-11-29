var blockchain = JSON.parse(document.getElementById("blockchain-getter").innerHTML);
document.getElementById("blockchain-getter").innerHTML = '';

var height = Object.keys(blockchain).length-1;

for (height; height >= Object.keys(blockchain).length-4; --height) {
    makeBlockchainDiv(height)
}
$(window).scroll(function () {
    if ($(window).scrollTop() == $(document).height() - $(window).height()) {
        makeBlockchainDiv(height)
        height -= 1;
    }
});

function makeBlockchainDiv(height) {
    transactions = blockchain[height]["transactions"];
    var total = 0
    for (let i = 0; i < transactions.length; ++i) {
        total += transactions[i]["amount"];
    }
    document.getElementById("main").innerHTML += "<div class='block-square'><p>hash=" +
        blockchain[height]['hash'] + "</p><p>height=" +
        height + "</p><p>time=" +
        blockchain[height]["time"] + "</p><p>Total amount=" +
        total.toString() + "</p></div>";
}

var link = $("<link />", {
    rel: "stylesheet",
    type: "text/css",
    href: "/stylesheets/blockchain.css"
})
$("head").append(link);