<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Stock Trading Game</title>
<style>
    body {
        margin: 0;
        background: #0f0f0f;
        color: #eaeaea;
        font-family: Arial, sans-serif;
        display: flex;
        flex-direction: column;
        height: 100vh;
    }

    .top-bar {
        display: flex;
        justify-content: flex-end;
        padding: 15px 25px;
        background: #1a1a1a;
        font-size: 18px;
    }

    .chart-container {
        flex: 1;
        padding: 20px;
    }

    canvas {
        background: #141414;
        border-radius: 10px;
    }

    .controls {
        display: flex;
        justify-content: center;
        gap: 20px;
        padding: 20px;
        background: #1a1a1a;
    }

    button {
        padding: 15px 30px;
        font-size: 18px;
        border: none;
        border-radius: 8px;
        cursor: pointer;
        transition: 0.2s;
    }

    .buy {
        background: #1db954;
        color: white;
    }

    .buy:hover {
        background: #17a74a;
    }

    .sell {
        background: #e53935;
        color: white;
    }

    .sell:hover {
        background: #c62828;
    }

    .info {
        position: absolute;
        top: 15px;
        left: 20px;
        font-size: 14px;
        color: #aaa;
    }
</style>
</head>
<body>

<div class="top-bar">
    <div id="balance">Balance: $1000</div>
</div>

<div class="info" id="info"></div>

<div class="chart-container">
    <canvas id="chart"></canvas>
</div>

<div class="controls">
    <button class="buy" onclick="buy()">BUY $100</button>
    <button class="sell" onclick="sell()">SELL $100</button>
</div>

<script>
const canvas = document.getElementById("chart");
const ctx = canvas.getContext("2d");

canvas.width = window.innerWidth - 40;
canvas.height = window.innerHeight * 0.6;

// Game state
let balance = 1000;
let shares = 0;
let price = 100;
let history = [];
const maxPoints = 120;

// Update price (random walk)
function updatePrice() {
    let volatility = 2;
    price += (Math.random() - 0.5) * volatility;
    if (price < 1) price = 1;

    history.push(price);
    if (history.length > maxPoints) history.shift();
}

// Draw chart
function drawChart() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    if (history.length < 2) return;

    let min = Math.min(...history);
    let max = Math.max(...history);

    let padding = 20;
    let width = canvas.width - padding * 2;
    let height = canvas.height - padding * 2;

    ctx.beginPath();

    history.forEach((p, i) => {
        let x = padding + (i / (maxPoints - 1)) * width;
        let y = padding + height - ((p - min) / (max - min)) * height;

        if (i === 0) ctx.moveTo(x, y);
        else ctx.lineTo(x, y);
    });

    // Color based on trend
    if (history[history.length - 1] > history[0]) {
        ctx.strokeStyle = "#1db954";
    } else {
        ctx.strokeStyle = "#e53935";
    }

    ctx.lineWidth = 2;
    ctx.stroke();
}

// Buy
function buy() {
    if (balance >= 100) {
        let boughtShares = 100 / price;
        shares += boughtShares;
        balance -= 100;
        updateUI();
    }
}

// Sell
function sell() {
    let sharesToSell = 100 / price;

    if (shares <= 0) return;

    if (shares < sharesToSell) {
        balance += shares * price;
        shares = 0;
    } else {
        shares -= sharesToSell;
        balance += 100;
    }

    updateUI();
}

// UI update
function updateUI() {
    document.getElementById("balance").innerText = 
        "Balance: $" + balance.toFixed(2);

    document.getElementById("info").innerText =
        `Price: $${price.toFixed(2)} | Shares: ${shares.toFixed(4)} | Net Worth: $${(balance + shares * price).toFixed(2)}`;
}

// Main loop
function loop() {
    updatePrice();
    drawChart();
    updateUI();
}

setInterval(loop, 200);

// Resize handling
window.addEventListener("resize", () => {
    canvas.width = window.innerWidth - 40;
    canvas.height = window.innerHeight * 0.6;
});
</script>

</body>
</html>