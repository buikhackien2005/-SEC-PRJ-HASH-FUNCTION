const { ipcRenderer } = require('electron');

// Lấy tham chiếu các phần tử điều khiển chính
const btnHash = document.getElementById('btn-hash');
const btnCancel = document.getElementById('btn-cancel');
const outputText = document.getElementById('output-text');
const outputEncoding = document.getElementById('output-encoding');
const inputMode = document.getElementById('input-mode');

// Lấy tham chiếu các container nhập liệu
const textContainer = document.getElementById('input-text');
const fileContainer = document.getElementById('input-file-container');
const fileSelector = document.getElementById('file-selector');
const filePathDisplay = document.getElementById('file-path-display');

// Lấy tham chiếu các phần tử của Thanh tiến trình
const progressContainer = document.getElementById('progress-container');
const progressBar = document.getElementById('progress-bar');
const progressText = document.getElementById('progress-text');

// Trạng thái cục bộ của UI
let currentFilePath = "";
let currentAlgo = "sha512"; // Mặc định khởi đầu là SHA-512

// ==========================================
// 1. XỬ LÝ SỰ KIỆN SIDEBAR CHỌN THUẬT TOÁN
// ==========================================
document.querySelectorAll('.algo-list li').forEach(li => {
    li.addEventListener('click', (e) => {
        // Nếu nút đang bị disabled (chưa cài thuật toán) thì bỏ qua
        if (e.target.classList.contains('disabled')) return;

        // Xóa class active cũ, gán active cho phần tử vừa click
        document.querySelectorAll('.algo-list li').forEach(el => el.classList.remove('active'));
        e.target.classList.add('active');
        
        // Cập nhật thuật toán hiện tại
        currentAlgo = e.target.getAttribute('data-algo');
    });
});

// ==========================================
// 2. CHUYỂN ĐỔI CHẾ ĐỘ NHẬP LIỆU (TEXT / FILE)
// ==========================================
inputMode.addEventListener('change', (e) => {
    if (e.target.value === 'file') {
        textContainer.style.display = 'none';
        fileContainer.style.display = 'flex';
    } else {
        textContainer.style.display = 'block';
        fileContainer.style.display = 'none';
    }
});

// Lắng nghe hành vi chọn file của người dùng
fileSelector.addEventListener('change', (e) => {
    if (e.target.files.length > 0) {
        // Lấy đường dẫn vật lý thực tế trên ổ cứng nhờ node integration
        currentFilePath = e.target.files[0].path; 
        filePathDisplay.innerText = "Selected: " + currentFilePath;
    } else {
        currentFilePath = "";
        filePathDisplay.innerText = "";
    }
});

// ==========================================
// 3. CƠ CHẾ HỨNG TIẾN ĐỘ (%) TỪ LUỒNG C++ ĐỔ VỀ
// ==========================================
ipcRenderer.on('hash-progress', (event, percentage) => {
    const pct = percentage.toFixed(1);
    progressBar.style.width = pct + '%';
    progressText.innerText = pct + '%';
});

// Lắng nghe sự kiện người dùng bấm nút Cancel khẩn cấp
btnCancel.addEventListener('click', () => {
    ipcRenderer.send('cancel-hash'); // Bắn tín hiệu sang main process để gạt cờ atomic
    btnCancel.innerText = "Canceling...";
    btnCancel.disabled = true;
});

// ==========================================
// 4. KHỞI CHẠY TIẾN TRÌNH BĂM DỮ LIỆU
// ==========================================
btnHash.addEventListener('click', async () => {
    const isFileMode = (inputMode.value === 'file');
    const encoding = outputEncoding.value;
    let dataToSend = "";

    // Kiểm tra dữ liệu đầu vào hợp lệ
    if (isFileMode) {
        if (!currentFilePath) {
            outputText.value = "Vui lòng chọn một file trước!";
            return;
        }
        dataToSend = currentFilePath;
    } else {
        dataToSend = textContainer.value;
    }

    // Thiết lập trạng thái UI khi bắt đầu xử lý ngầm
    btnHash.style.display = 'none';      // Ẩn nút Hash
    btnCancel.style.display = 'block';   // Hiện nút Cancel
    btnCancel.innerText = "Cancel";
    btnCancel.disabled = false;

    // Reset lại thanh tiến trình về 0%
    progressContainer.style.display = 'block';
    progressBar.style.width = '0%';
    progressText.innerText = '0%';
    outputText.value = "";

    try {
        // Gửi yêu cầu bất đồng bộ xuống luồng ngầm qua IPC
        const result = await ipcRenderer.invoke('hash-data', dataToSend, encoding, isFileMode, currentAlgo);
        outputText.value = result;
    } catch (err) {
        // Kiểm tra xem lỗi trả về có phải do chủ động kích hoạt lệnh Hủy không
        if (err.message.includes("PROCESS_CANCELLED")) {
            outputText.value = "⚠️ Đã hủy bỏ tiến trình tính toán theo yêu cầu người dùng.";
        } else {
            outputText.value = "Lỗi hệ thống: " + err.message;
        }
    } finally {
        // Khôi phục lại trạng thái giao diện gốc ban đầu
        btnHash.style.display = 'block';
        btnCancel.style.display = 'none';
        
        // Cho thanh tiến trình hiển thị thêm 1.5 giây để tạo cảm giác mượt mà trước khi ẩn
        setTimeout(() => { 
            progressContainer.style.display = 'none'; 
        }, 1500);
    }
});