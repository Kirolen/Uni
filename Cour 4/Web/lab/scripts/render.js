const content = document.getElementById("content");
const pageNav = document.getElementById("page-nav");

const cpu = document.getElementById("cpu");
const intel = document.getElementById("intel");
const cpuAMD = document.getElementById("cpu-amd");

const gpu = document.getElementById("gpu");
const nvidia = document.getElementById("nvidia");
const gpuAMD = document.getElementById("gpu-amd");

const motherboards = document.getElementById("motherboards");
const ram = document.getElementById("ram");
const storage = document.getElementById("storage")
const powerSupplies = document.getElementById("power-supplies")


let type = "cpu";
let subType = "all";
let page = 0;

const imageUrl = "https://wallpapercave.com/wp/wp5764222.jpg";
const itemsPerPage = 6;

import { CPUData, GPUData, MotherboardData, RAMData, StorageData, PowerSupplyData } from "./data/Data.js";

function getFilteredProducts() {
    switch (type) {
        case "cpu":
            return subType === "all" ? CPUData : CPUData.filter(cpu => cpu.cpuType === subType);
        case "gpu":
            return subType === "all" ? GPUData : GPUData.filter(gpu => gpu.gpuType === subType);
        case "ram":
            return RAMData;
        case "motherboards":
            return MotherboardData;
        case "storage":
            return StorageData;
        case "power-supplies":
            return PowerSupplyData;
        default:
            return [];
    }
}

function renderContent() {
    content.innerHTML = "";
    const filteredProducts = getFilteredProducts();

    let start = page * itemsPerPage;
    let visibleProducts = filteredProducts.slice(start, start + itemsPerPage);

    visibleProducts.forEach(product => {
        const card = document.createElement("div");
        card.classList.add("card");
        card.innerHTML = `
            <img src="${product?.imageUrl}" alt="Product Image">
            <p>${product.name}</p>
            <p class="stock">In Stock</p>
            <h1>${product.price}</h1>
        `;
        content.appendChild(card);
    });

    updatePaginationButtons();
}

function renderPagination() {
    pageNav.innerHTML = "";
    const filteredProducts = getFilteredProducts();
    const numPages = Math.ceil(filteredProducts.length / itemsPerPage);

    for (let i = 0; i < numPages; i++) {
        const button = document.createElement("button");
        button.textContent = i + 1;
        button.classList.add("page-button");
        if (i === page) button.classList.add("active");

        button.addEventListener("click", () => {
            page = i;
            renderContent();
        });

        pageNav.appendChild(button);
    }
}

function updatePaginationButtons() {
    const buttons = document.querySelectorAll(".page-button");
    buttons.forEach((button, index) => {
        button.classList.toggle("active", index === page);
    });
}

function setType(newType) {
    if (type !== newType) {
        type = newType;
        subType = "all";
        page = 0;
        renderPagination();
    }
    renderContent();
}

function setSubType(newSubType) {
    subType = newSubType;
    page = 0;
    renderPagination();
    renderContent();
}

cpu.addEventListener("click", event => {
    setType("cpu");
    setSubType('all')
    event.stopPropagation();
});

intel.addEventListener("click", event => {
    type = "cpu";
    setSubType("intel");
    event.stopPropagation();
});

cpuAMD.addEventListener("click", event => {
    type = "cpu";
    setSubType("amd");
    event.stopPropagation();
});

gpu.addEventListener("click", event => {
    setType("gpu");
    setSubType('all')
    event.stopPropagation();
});

nvidia.addEventListener("click", event => {
    type = "gpu";
    setSubType("nvidia");
    event.stopPropagation();
});

gpuAMD.addEventListener("click", event => {
    type = "gpu";
    setSubType("amd");
    event.stopPropagation();
});

motherboards.addEventListener("click", event => {
    setType("motherboards");
    event.stopPropagation();
});

ram.addEventListener("click", event => {
    setType("ram");
    event.stopPropagation();
});

storage.addEventListener("click", event => {
    setType("storage");
    event.stopPropagation();
});

powerSupplies.addEventListener("click", event => {
    setType("power-supplies");
    event.stopPropagation();
});

renderPagination();
renderContent();
