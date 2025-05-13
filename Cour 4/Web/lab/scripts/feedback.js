const feedbackForm = document.getElementById("feedback");
const feedbackClose = document.getElementById("feedback-close")
const feedbackToggle = document.getElementById("feedback-toggle");
const feedbackFormEl = document.querySelector("#feedback form");

console.log("feedback: ", feedbackFormEl)

const body = document.body;

let feedbackIsOpen = false; 

function setFeedbackToggle() {
    feedbackIsOpen = !feedbackIsOpen; 
    if (feedbackForm) {
        if (feedbackIsOpen) feedbackForm.classList.add("flex"); 
        else feedbackForm.classList.remove("flex");
    }
}

if (feedbackToggle) {
    feedbackToggle.addEventListener("click", setFeedbackToggle);
}

if (feedbackClose) {
    feedbackClose.addEventListener("click", setFeedbackToggle);
}

if (feedbackFormEl) {
    feedbackFormEl.addEventListener("submit", function (e) {
        console.log('valid')
        e.preventDefault();
        let valid = true;

        const fields = feedbackFormEl.querySelectorAll("input[required], textarea[required]");

        fields.forEach(field => {
            if (!field.value.trim()) {
                field.classList.add("error");
                field.placeholder = "Поле є обов’язковим для заповнення";
                valid = false;
            } else {
                field.classList.remove("error");
            }
        });

        if (valid) {
            alert("Форма успішно відправлена!");
            feedbackFormEl.reset();
            fields.forEach(f => f.classList.remove("error"));
        }
    });

    feedbackFormEl.querySelectorAll("input, textarea").forEach(field => {
        field.addEventListener("input", () => {
            if (field.value.trim()) {
                field.classList.remove("error");
            }
        });
    });
}