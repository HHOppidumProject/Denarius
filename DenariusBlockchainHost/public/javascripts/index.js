String.prototype.toProperCase = function () {
    return this.replace(/\w\S*/g, function (txt) { return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase(); });
};

var link = $("<link />", {
    rel: "stylesheet",
    type: "text/css",
    href: "/stylesheets/index.css"
})
$("head").append(link);

$(document).ready(function () {
    new ResizeObserver(resizeChart).observe(main);
    var currencyList;
    var currentCurrency = ["British Pound", "gbp"];


    GetCurrencies();


    google.charts.load('current', { callback: drawBasic, 'packages': ['table', 'corechart', 'line'] });


    var div = document.getElementById("content");
    var bottomOfContentsDiv = div.getBoundingClientRect().top + div.clientHeight;
    var toBottomOfPage = (Math.max(document.documentElement.clientHeight || 0, window.innerHeight || 0)) - bottomOfContentsDiv;
    var tableDiv = document.getElementById("Table");
    if (Math.max(document.documentElement.clientWidth || 0, window.innerWidth || 0) > 1100) {
        var ElementSize = toBottomOfPage / 2;
        tableDiv.style.maxHeight = `${ElementSize}px`;
    } else {
        var ElementSize = Math.max(document.documentElement.clientHeight || 0, window.innerHeight || 0)*0.35;
    }


    var ExchangeRate = GetExchangeRate(currentCurrency[1]);
    UpdateChange();

    CurrencyDropdown = $('#select2Currency')
    Select2CD = CurrencyDropdown.select2({
        height: "2rem",
        debug: true
    });
    CurrencyDropdown.on('select2:select', Selection);
    CurrencyDropdown.on('select2:change', Selection);

    function Selection(e) {
        ExchangeRate = GetExchangeRate(e.params.data["id"]);
        drawBasic(e.params.data["text"]);
        UpdateChange();
    }

    function resizeChart() {
        drawBasic(currentCurrency[0]);
        Select2CD.val(currentCurrency[1]);
        Select2CD.trigger("change")
    }

    function drawBasic(currency = "British Pound") {

        currentCurrency = [currency, currencyList.find(x => x["name"].toProperCase() == currency)["cc"].toLowerCase()];

        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Date');
        data.addColumn('number', 'Value');

        data.addRows(ExchangeRate);

        var options = {
            fontName: "Trajan",
            backgroundColor: '#FFFCF0',
            chartArea: {
                backgroundColor: "#FFFCF0"
            },
            title: `Exchange Rate Solidus to the ${currency}`,
            series: {
                // Gives each series an axis name that matches the Y-axis below.
                0: { axis: 'Value' },
            },
            height: ElementSize,
            vAxis: {
                title: "Value",
                format: he.decode(he.encode(currencyList.find(x => x["name"].toProperCase() == currency)["symbol"])) + "####.####"

            }
        };

        var chart = new google.visualization.LineChart(document.getElementById('chart_div'));

        chart.draw(data, options);
    }

    function GetExchangeRate(currency2, currency1 = "usd") {
        var now = new Date();
        var exchangeRates = [];
        var xmlHttp = new XMLHttpRequest();
        for (var i = new Date(now.getFullYear(), now.getMonth() - 1, now.getDate()); i <= now; i.setDate(i.getDate() + 1)) {
            if (i.toISOString().split('T')[0] != now.toISOString().split('T')[0]) {
                xmlHttp.open("GET", "https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/" + i.toISOString().split('T')[0] + `/currencies/${currency1}/${currency2}.json`, false); // false for synchronous request
            } else {
                xmlHttp.open("GET", `https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/latest/currencies/${currency1}/${currency2}.json`, false); // false for synchronous request
            }
            xmlHttp.send();
            var rateForDay = JSON.parse(xmlHttp.responseText);
            exchangeRates.push([rateForDay["date"], rateForDay[currency2] * 17.136]);
        }
        return exchangeRates;
    }

    function GetCurrencies() {
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.open("GET", "https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/other/currencies2.json", false); // false for synchronous request
        xmlHttp.send();
        currencyList = JSON.parse(xmlHttp.responseText);
        for (var i of currencyList) {
            document.getElementById("select2Currency").innerHTML += "<option value='" + i['cc'].toLowerCase() + "'>" + i['name'].toProperCase() + "</option>"
        }
    }

    function UpdateChange() {
        var changeYesterday = (((ExchangeRate[ExchangeRate.length - 1][1] - ExchangeRate[ExchangeRate.length - 2][1]) / ExchangeRate[ExchangeRate.length - 2][1]) * 100);
        var perChangeHeading = document.getElementById("PercentChange");
        perChangeHeading.innerHTML = changeYesterday.toString() + "%";
        if (changeYesterday < 0) {
            perChangeHeading.style.color = "#960018";
        } else if (changeYesterday == 0) {
            perChangeHeading.style.color = "inherit";
        } else if (changeYesterday > 0) {
            perChangeHeading.style.color = "#427131";
        }
        var todaysvalue = document.getElementById("valueToday");
        todaysvalue.innerHTML = currencyList.find(x => x["name"].toProperCase() == currentCurrency[0])["symbol"] + ExchangeRate[ExchangeRate.length - 1][1].toString()
    }
});