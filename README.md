# 🔒 SHA-512 C++ Implementation

## 📌 Giới thiệu
Dự án này là bản cài đặt hoàn chỉnh thuật toán băm mật mã **SHA-512** bằng ngôn ngữ C++. 

Mục tiêu của bài tập lớn nhằm minh họa và kiểm chứng sâu sát các cơ sở toán học, thao tác bitwise (dịch bit, xoay bit), và kiến trúc nhào nặn dữ liệu (80 vòng lặp Compression Function) cốt lõi của họ thuật toán SHA-2.

## 👥 Thành viên nhóm
* **Thành viên 1:** (Cao Quang Hưng - 20234012) - *Tìm hiểu về định nghĩa, tính chất, ứng dụng thực tế của Hàm băm. Xác định Input - Output, vẽ sơ đồ khối tổng thể và mô tả luồng xử lý dữ liệu của thuật toán SHA - 512. Lập trình phần primitives & math và padding & parsing.*
* **Thành viên 2:** (Kiều Việt Hoàng - 20236179) - *Mô tả chi tiết từng bước xử lý của thuật toán. Trình bày Input - Output của từng bước nhỏ và phân tích sự biến đổi dữ liệu sau từng bước. Lập trình phần padding & parsing.*
* **Thành viên 3:** (Bùi Khắc Kiên - 20234016) - *Tổng hợp những nội dung đã tìm hiểu và nghiên cứu được về Hàm băm và đưa vào Powerpoint trình bày. Lập trình phần 80 rounds và bổ sung việc in ra các giá trị trung gian sau từng bước nhằm phục vụ cho Testing.*
* **Thành viên 4:** (Nguyễn Quốc Huy - 20234013) - *Chạy mô phỏng với dữ liệu thật, thu thập Input - Output ở từng bước nhỏ. Lập trình phân main.*
* **Thành viên 5:** (Nguyễn Minh Hùng - 20234011) - *Chuẩn bị test vector chuẩn và đối chiếu chúng với các giá trị trung gian được thu thập. Lập trình phần main.*
* **Thành viên 6:** (Mạc Quang Huy - 20213709) - *Quản lý tiến độ commit code của từng thành viên.*

## ✨ Tính năng nổi bật
* **Padding & Parsing:** Tự động chèn bit đệm và cắt khối dữ liệu chuẩn xác 1024-bit.
* **Hệ thống Logging chi tiết:** Hỗ trợ xuất luồng dữ liệu trung gian (Message Schedule, 80 Rounds, thanh ghi a-h) ra file `.txt` để phục vụ Debug và chứng minh tính đúng đắn của thuật toán.
* **Test Cases chuẩn NIST:** Đã vượt qua các kịch bản kiểm thử kinh điển (Chuỗi rỗng, Hiệu ứng thác đổ, Văn bản cực dài).

## 🛠 Yêu cầu hệ thống
* Hệ điều hành: Linux/Ubuntu hoặc Windows.
* Trình biên dịch: `g++` (hỗ trợ chuẩn C++11 trở lên).
* Công cụ quản lý phiên bản: Git.

## 🚀 Cách biên dịch và chạy chương trình

**Bước 1: Clone kho lưu trữ về máy**
```bash
git clone https://github.com/buikhackien2005/-SEC-PRJ-HASH-FUNCTION
cd your-repo-name
```

**Bước 2: Biên dịch mã nguồn bằng g++**
```bash
g++ hash_main.cpp -o sha512.exe
```

**Bước 3: Chạy chương trình**
```bash
./sha512.exe
```

## 📝 Cấu trúc mã nguồn chính
* `hash_primitives_math.cpp`: Chứa các hàm toán học được sử dụng và các giá trị H khởi tạo.
* `hash_padding_parsing.cpp`: Chứa các phương thức tiền xử lý và ép kiểu hiển thị.
* `hash_80_rounds.cpp`: Chứa các bước xử lý 1 khối Mi trong mỗi vòng lặp.
* `hash_main.cpp`: Chứa hàm điều phối chính và menu giao diện Console.

---
*Bài tập lớn thuộc học phần An toàn thông tin.*