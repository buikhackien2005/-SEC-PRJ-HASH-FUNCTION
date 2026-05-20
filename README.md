# SHA-GUI-App: Bộ Công Cụ Mã Hóa Toàn Vẹn Dữ Liệu Cấp Công Nghiệp

SHA-GUI-App là một ứng dụng Desktop hiệu năng cao được xây dựng trên sự kết hợp giữa **Electron** (Frontend/Giao diện) và lõi xử lý mã nguồn nhị phân **C++ Native Addon** thông qua **Node-API (napi-h)**. Ứng dụng cung cấp giải pháp băm dữ liệu tốc độ cao, xử lý luồng (streaming) tối ưu hóa tài nguyên phần cứng, và đảm bảo giao diện luôn mượt mà phản hồi kể cả khi tính toán các tệp tin có dung lượng lên đến hàng trăm Gigabyte.

---

## 🚀 Tính Năng Cốt Lõi & Kiến Trúc Hệ Thống

1. **Kiến Trúc Luồng Đọc File Hạn Chế Tối Đa RAM (Industrial Streaming Engine)**
   - Thay vì tải toàn bộ nội dung tệp tin vào bộ nhớ đệm (gây tràn RAM với file lớn), ứng dụng sử dụng `std::ifstream` để đọc dữ liệu dưới dạng luồng nhị phân theo từng khối nhỏ **8KB (Chunk-based streaming)**. 
   - Điều này đảm bảo mức tiêu thụ bộ nhớ RAM của phần mềm luôn giữ ở mức hằng số tối thiểu cố định (vài mươi Megabytes) bất kể tệp tin đầu vào nặng bao nhiêu.

2. **Xử Lý Đa Luồng Bất Đồng Bộ (`Napi::AsyncProgressQueueWorker`)**
   - Các tác vụ tính toán toán học mật mã nặng nề được đẩy hoàn toàn từ Luồng chính (Main Thread) xuống Luồng nền (Background Worker Thread) của hệ điều hành.
   - Nhờ vậy, luồng giao diện người dùng luôn được giải phóng để đạt tốc độ phản hồi 60fps ổn định, loại bỏ hoàn toàn hiện tượng cửa sổ bị đơ, treo hoặc rơi vào trạng thái *Not Responding*.

3. **Cơ Chế Phanh Khẩn Cấp An Toàn Luồng (Atomic Cancel Mechanism)**
   - Tích hợp biến cờ an toàn luồng `std::atomic<bool> global_cancel_flag` nhằm quản lý vòng đời tác vụ. 
   - Khi người dùng nhấn nút **Cancel** trên UI, một tín hiệu IPC sẽ được bắn xuống lõi C++ để lập tức ngắt vòng lặp đọc file, đóng luồng dữ liệu an toàn và giải phóng bộ khóa tệp tin (file lock) tức thì.

4. **Chống Nghẽn Cổ Chai Giao Tiếp IPC (Progress Throttling)**
   - Để tránh làm nghẽn luồng truyền tin (IPC) giữa tiến trình con và tiến trình hiển thị do gửi dữ liệu quá dày đặc, lõi C++ chỉ phát tín hiệu cập nhật thanh tiến trình (`Progress Bar`) khi tỷ lệ phần trăm xử lý tăng lên $\ge 1\%$ hoặc khi đạt mốc chốt sổ $100\%$.

---

## 🔐 Các Thuật Toán Mã Hóa Được Hỗ Trợ

Ứng dụng tích hợp thành công cấu trúc điều phối thuật toán (Routing Engine) linh hoạt với cỗ máy trạng thái (State Machine) riêng biệt cho từng chuẩn mật mã:

* **SHA-512**: Triển khai theo chuẩn toán học 64-bit Word Machine (Big-Endian).
* **SHA-256**: Triển khai tối ưu hóa trên cấu trúc từ 32-bit (Big-Endian) với khối đệm 64 bytes.
* **MD5**: Hỗ trợ giải pháp kiểm tra toàn vẹn truyền thống, tối ưu hóa xử lý trên kiến trúc Little-Endian độc lập (lật byte ở đầu ra Hex).
* **SHA-1**: Hỗ trợ băm chuỗi/tệp tin ra định dạng 160-bit truyền thống.
* **SHA3-256 (Keccak)**: Sử dụng mô hình cấu trúc bọt biển hoàn toàn mới (**Sponge Construction** với trạng thái khổng lồ 1600-bit) thay thế cho mô hình Merkle-Damgård cổ điển, áp dụng quy tắc padding `0x06` chuẩn FIPS 202 quốc tế.

---

## 📂 Cấu Trúc Thư Mục Dự Án (Clean Architecture)

Mã nguồn được phân tách mô-đun hóa sạch sẽ (Decoupled & Encapsulated) theo từng thư mục không gian tên riêng biệt:

```text
SHA-GUI-App/
├── binding.gyp          # File bản vẽ cấu hình biên dịch Node-GYP cho lõi C++
├── package.json         # Cấu hình dự án và quản lý các phụ thuộc Electron/Node
├── src/                 # Mã nguồn hạt nhân C++ Native Addon
│   ├── addon.cpp        # Cầu nối điều phối, quản lý luồng ngầm Async và cổng ra vào IPC
│   ├── md5/             # Lõi logic thuật toán MD5 (Little-Endian)
│   ├── sha1/            # Lõi logic thuật toán SHA-1 (5 thanh ghi 32-bit)
│   ├── sha256/          # Lõi logic thuật toán SHA-256 (32-bit Word)
│   ├── sha512/          # Lõi logic thuật toán SHA-512 (64-bit Word)
│   ├── keccak/          # Lõi logic thuật toán SHA3-256 (Sponge f-1600)
│   └── utils/           # Các hàm toán học, thao tác bit và cấu trúc dùng chung
└── ui/                  # Giao diện người dùng (Frontend Electron)
    ├── index.html       # Layout cấu trúc ứng dụng (Sidebar, Cấu hình, Vùng dữ liệu)
    ├── main.js          # Tiến trình chính Electron (Main Process), quản lý cửa sổ và IPC
    ├── renderer.js      # Tiến trình hiển thị (Renderer Process), bắt tương tác UI
    └── style.css        # Định dạng thẩm mỹ giao diện