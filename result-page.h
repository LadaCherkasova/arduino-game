char resultPage[] PROGMEM = R"=====(
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
        padding: 24px;
        margin: 200px 300px;
        display: flex;
        flex-direction: column;
    }
    a.action-button {
        border-radius: 16px;
        padding: 12px;
        border: 1px solid #C1C1C1;
        background: transparent;
        margin-top: 20px;
        font-size: 20px;
        width: fit-content;
        color: unset;
        text-decoration: unset;
    }
    .answer-field {
        background: rgba(249, 237, 137, 0.85);
        border: 1px solid #B7AD5A;
        backdrop-filter: blur(50px);
        padding: 20px;
        border-radius: 8px;
        display: flex;
        width: fit-content;
    }
    .title {
        margin-top: 0;
        font-size: 24px;
        margin-bottom: 8px;
    }
</style>
<body>

<div class="container">
    <p class="title">Initial sequence: </p>
    <div class="answer-field" id="initialSequenceField"></div>

    <p class="title" style="margin-top: 20px">Answer sequence: </p>
    <div class="answer-field" id="checkSequenceField"></div>

    <a class="action-button" href="../start">Try again</a>
</div>

<script>
    let initialSequence = "";
    let checkSequence = "";
    let steps = 0;

    setInterval(function() {
      getCurrentState();
    }, 500);

    function getCurrentState() {
      let xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
          
        if (this.status == 200) {
          [stepsStr, initialSequence, checkSequence] = this.responseText.split([' ']);
          steps = parseInt(stepsStr);
        
          if (steps === 0 || initialSequence === "") {
            return;
          }
          
          display('initialSequence', initialSequence);
          display('checkSequence', checkSequence);
        }
      };
      xhttp.open("GET", "getCurrentState", true);
      xhttp.send();
    }
      
    function display(id, sequence) {
        let finalHTML = "";
        for (let i = 1; i <= steps; i++) {
            let colorInfo;
            if (sequence[i-1] === '1') {
                colorInfo = ` style=\"background: rgba(246, 49, 49, 0.7);border: 1px solid #C06565;\"`
            }
            if (sequence[i-1] === '2') {
                colorInfo = ` style=\"background: rgba(82, 215, 60, 0.7);border: 1px solid #4AA73A;\"`
            }
            if (sequence[i-1] === '3') {
                colorInfo = ` style=\"background: rgba(36, 57, 243, 0.7);border: 1px solid #2030BD;\"`
            }

            finalHTML += `<style>.answer-item-wrap {display: flex;flex-direction: column;align-items: center;justify-content: center;margin-right: 24px;}.answer-item {width: 60px;height: 60px;background: rgba(255, 255, 255, 0.7);border: 1px solid #808080;border-radius: 100%;margin-bottom: 10px;}.index {font-size: 16px;line-height: 19px;text-align: center;color: #000000;}  `
                + `</style> <body> <div class=\"answer-item-wrap\"> <div class=\"answer-item\" `
                + colorInfo
                + `></div><span class=\"index\">`
                + i + `</span></div>`;
        }

        document.getElementById(id + "Field").innerHTML = finalHTML;
    }
</script>
</body>
</html>
)=====";
