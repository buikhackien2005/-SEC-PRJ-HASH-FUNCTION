const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');

// Nạp lõi C++ siêu tốc của bạn vào đây!
const addon = require('../build/Release/sha512_addon.node');

function createWindow() {
    const win = new BrowserWindow({
        width: 1000,
        height: 700,
        autoHideMenuBar: true, // Giấu thanh menu mặc định cho đẹp
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false // Bật tính năng gọi Node.js trực tiếp từ giao diện
        }
    });

    win.loadFile('ui/index.html');
}

app.whenReady().then(createWindow);

// Thêm tham số isFile vào hàm lắng nghe
ipcMain.handle('hash-data', async (event, inputData, outputEncoding, isFile) => {
    try {
        // Truyền thêm isFile xuống C++
        const hexResult = await addon.hash(inputData, isFile);

        if (outputEncoding === 'hex_upper') {
            return hexResult.toUpperCase();
        } else if (outputEncoding === 'base64') {
            return Buffer.from(hexResult, 'hex').toString('base64');
        }
        
        return hexResult; 
    } catch (err) {
        return "Lỗi nội bộ: " + err.message;
    }
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') app.quit();
});