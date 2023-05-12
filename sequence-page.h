const char sequencePage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
    html {
        background-color: rgba(255, 248, 169, 0.4);
    }
    .container {
        background: rgba(255, 255, 255, 0.8);
        backdrop-filter: blur(50px);
        border-radius: 16px;
        padding: 16px;
        margin: 200px 300px;
    }
    .header {
        display: flex;
        justify-content: end;
    }
    .action-button {
        width: fit-content;
        padding: 12px;
        border-radius: 16px;
        border: 1px solid #C1C1C1;
        margin-left: 12px;
        background: transparent;
        margin-bottom: 40px;
        cursor: pointer;
    }
    .answer-field {
        background: rgba(249, 237, 137, 0.85);
        border: 1px solid #B7AD5A;
        backdrop-filter: blur(50px);
        padding: 4px 80px 4px 20px;
        border-radius: 8px;
        display: flex;
        flex-wrap: wrap;
    }
    .answer-field-wrap {
        position: relative;
    }
    .small-button {
        position: absolute;
        right: 0;
        padding: 4px 8px;
        border-radius: 16px;
        border: 1px solid #C1C1C1;
        background: rgba(255, 255, 255, 0.8);
        backdrop-filter: blur(50px);
        cursor: pointer;
    }
    .plus-button {
        top: 30%;
    }
    .minus-button {
        bottom: 30%;
    }
</style>
<body>

<div class="container">
    <div class="header">
        <button class="action-button" onClick="generate()">Сгенерировать</button>
        <button class="action-button" onClick="sendSequence()">Отправить</button>
    </div>

    <div class="answer-field-wrap" onCLick="adjustSequence(event)">
        <div class="answer-field" id="answer-field"></div>
        <button class="small-button plus-button" onClick="changeStepsAmount(true)">+ шаг</button>
        <button class="small-button minus-button" onClick="changeStepsAmount(false)">- шаг</button>
    </div>
</div>

<script>
    let steps = 8;
    let sequence = "";

    const redBg = 'rgba(246, 49, 49, 0.7)';
    const greenBg = 'rgba(82, 215, 60, 0.7)';
    const blueBg = 'rgba(36, 57, 243, 0.7)';

    const redStyle = 'background: rgba(246, 49, 49, 0.7);border: 1px solid #C06565;';
    const greenStyle = 'background: rgba(82, 215, 60, 0.7);border: 1px solid #4AA73A;';
    const blueStyle = 'background: rgba(36, 57, 243, 0.7);border: 1px solid #2030BD;';

    display();

    function generate() {
        let sequence = [];
        for (let i = 1; i <= steps; i++) {
            sequence.push(Math.floor(Math.random() * 3) + 1)
        }
        this.sequence = sequence.join('');
        display();
    }

    function display() {
        if (!this.sequence || !this.sequence.trim()) {
            this.sequence = ' '.repeat(steps);
        }
        let finalHTML = "";
        for (let i = 1; i <= steps; i++) {
            let colorInfo;
            if (this.sequence[i-1] === '1') {
                colorInfo = ` style=\"${redStyle}\"`
            }
            if (this.sequence[i-1] === '2') {
                colorInfo = ` style=\"${greenStyle}\"`
            }
            if (this.sequence[i-1] === '3') {
                colorInfo = ` style=\"${blueStyle}\"`
            }

            // answer item html code
            finalHTML += `<style>.answer-item-wrap {display: flex;flex-direction: column;align-items: center;justify-content: `
                + `center;margin: 16px 24px 16px 0;;}.answer-item {cursor: pointer;width: 60px;height: 60px;background: `
                + `rgba(255, 255, 255, 0.7);border: 1px solid #808080;border-radius: 100%;margin-bottom: 10px;}.index `
                + `{font-size: 16px;line-height: 19px;text-align: center;color: #000000;}  `
                + `</style> <body> <div class=\"answer-item-wrap\"> <button class=\"answer-item\" `
                + colorInfo
                + `></button><span class=\"index\">`
                + i
                + `</span></div>`;
        }

        document.getElementById("answer-field").innerHTML = finalHTML;
    }

    function changeStepsAmount(increase) {
        if (increase) {
            steps += 1;
            this.sequence = this.sequence.concat(' ');
        }
        if (!increase && steps > 0) {
            steps -= 1;
            this.sequence = this.sequence.slice(0, this.sequence.length - 1);
        }
        display();
    }

    function adjustSequence(click) {
        const initialBg = click.target.style.background;
        const index = +click.target.nextElementSibling?.innerText;

        if (initialBg === redBg) {
            this.sequence = this.sequence.slice(0, index - 1).concat(2).concat(this.sequence.slice(index));
            display();
        }
        if (initialBg === greenBg) {
            this.sequence = this.sequence.slice(0, index - 1).concat(3).concat(this.sequence.slice(index));
            display();
        }
        if (initialBg === blueBg) {
            this.sequence = this.sequence.slice(0, index - 1).concat(1).concat(this.sequence.slice(index));
            display();
        }
    }

    function sendSequence() {
        let xhr = new XMLHttpRequest();
        const body = 'sequence=' + encodeURIComponent(this.sequence);

        xhr.open("POST", "showSequence", true);
        xhr.send(body);
    }
</script>
</body>
</html>
)=====";
