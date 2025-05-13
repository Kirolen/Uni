const slider = document.querySelector(".slider");
const arrowLeft = document.querySelector(".arrow-left");
const arrowRight = document.querySelector(".arrow-right");
const slides = document.querySelectorAll(".slide");
const bottom = document.getElementById("bottom");

let currentSlideIndex = 0;
const paginationCircles = [];
let intervalId = null;

document.addEventListener("DOMContentLoaded", initializeSlider);

function initializeSlider() {
    showSlide();
    intervalId = setInterval(nextSlide, 4000);
}

function createPaginationCircles() {
    const div = document.createElement("div");
    div.className = "pagination-circle";
    bottom.appendChild(div);
    paginationCircles.push(div);
}

function addPagination() {
    slides.forEach(createPaginationCircles);
    paginationCircles[0].classList.add("active");
    paginationCircles.forEach((circle, index) => {
        circle.addEventListener("click", () => changeSlide(index));
    });
}

function addActive() {
    paginationCircles[currentSlideIndex].classList.add("active");
}

function removeActive() {
    paginationCircles[currentSlideIndex].classList.remove("active");
}

function showSlide() {
    slides[currentSlideIndex].classList.remove("fade-out");
    slides[currentSlideIndex].classList.add("block");
}

function hideSlide() {
    slides[currentSlideIndex].classList.add("fade-out");
    slides[currentSlideIndex].classList.remove("block");
}

function changeSlide(newIndex) {
    hideSlide();
    removeActive();
    currentSlideIndex = (newIndex + slides.length) % slides.length;
    showSlide();
    addActive();
    resetInterval();
}

function nextSlide() {
    changeSlide(currentSlideIndex + 1);
}

function prevSlide() {
    changeSlide(currentSlideIndex - 1);
}

function resetInterval() {
    clearInterval(intervalId);
    intervalId = setInterval(nextSlide, 4000);
}

addPagination();
arrowLeft.addEventListener("click", prevSlide);
arrowRight.addEventListener("click", nextSlide);