const { ipcRenderer } = require('electron');

const btnHash = document.getElementById('btn-hash');
const outputText = document.getElementById('output-text');
const outputEncoding = document.getElementById('output-encoding');
const inputMode = document.getElementById('input-mode');

// DOM Elements cho Text / File
const textContainer = document.getElementById('input-text');
const fileContainer = document.getElementById('input-file-container');
const fileSelector = document.getElementById('file-selector');
const filePathDisplay = document.getElementById('file-path-display');

let currentFilePath = "";

// 1. Lắng nghe sự kiện chuyển chế độ Text / File
inputMode.addEventListener('change', (e) => {
    if (e.target.value === 'file') {
        textContainer.style.display = 'none';
        fileContainer.style.display = 'flex';
    } else {
        textContainer.style.display = 'block';
        fileContainer.style.display = 'none';
    }
});

// 2. Lắng nghe khi người dùng chọn File
fileSelector.addEventListener('change', (e) => {
    if (e.target.files.length > 0) {
        // Nhờ tắt contextIsolation trong main.js, ta lấy được đường dẫn thật (path)
        currentFilePath = e.target.files[0].path; 
        filePathDisplay.innerText = "Selected: " + currentFilePath;
    } else {
        currentFilePath = "";
        filePathDisplay.innerText = "";
    }
});

// 3. Xử lý nút Bấm Hash!
btnHash.addEventListener('click', async () => {
    const isFileMode = (inputMode.value === 'file');
    const encoding = outputEncoding.value;
    let dataToSend = "";

    if (isFileMode) {
        if (!currentFilePath) {
            outputText.value = "Vui lòng chọn một file trước!";
            return;
        }
        dataToSend = currentFilePath;
    } else {
        dataToSend = textContainer.value;
    }

    // Hiển thị trạng thái đang xử lý (không làm đơ UI)
    outputText.value = "Đang xử lý ngầm (Async)... Vui lòng đợi!";
    btnHash.disabled = true;

    try {
        // Gọi C++ với 3 tham số
        const result = await ipcRenderer.invoke('hash-data', dataToSend, encoding, isFileMode);
        outputText.value = result;
    } catch (err) {
        outputText.value = "Lỗi: " + err;
    } finally {
        btnHash.disabled = false;
    }
});