#include <napi.h>
#include <fstream>
#include "sha2/sha512_core.h"

class HashWorker : public Napi::AsyncWorker {
public:
    HashWorker(Napi::Env& env, std::string input, bool is_file)
        : Napi::AsyncWorker(env), input(input), is_file(is_file), deferred(Napi::Promise::Deferred::New(env)) {}

    Napi::Promise GetPromise() { return deferred.Promise(); }

protected:
    void Execute() override {
        sha512::SHA512 hasher;

        try {
            if (is_file) {
                // Chế độ băm File cuốn chiếu (Streaming)
                std::ifstream file(input, std::ios::binary);
                if (!file.is_open()) {
                    throw std::runtime_error("Lỗi: Không thể mở file. Hãy kiểm tra đường dẫn.");
                }

                uint8_t file_buffer[8192]; // Đọc từng cục 8KB
                while (file.read(reinterpret_cast<char*>(file_buffer), sizeof(file_buffer))) {
                    hasher.update(file_buffer, file.gcount());
                }
                // Xử lý nốt phần còn sót lại (nếu có)
                if (file.gcount() > 0) {
                    hasher.update(file_buffer, file.gcount());
                }
            } else {
                // Chế độ băm chuỗi văn bản
                hasher.update(reinterpret_cast<const uint8_t*>(input.data()), input.length());
            }

            hash_result = hasher.finalize();

        } catch (const std::exception& e) {
            SetError(e.what());
        }
    }

    void OnOK() override {
        Napi::Env env = Env();
        deferred.Resolve(Napi::String::New(env, hash_result));
    }

    void OnError(const Napi::Error& e) override {
        deferred.Reject(e.Value());
    }

private:
    std::string input;
    bool is_file;
    std::string hash_result;
    Napi::Promise::Deferred deferred;
};

// Hàm Wrapper bộc lộ ra ngoài nhận 2 tham số: (input_string, is_file_boolean)
Napi::Value HashSHA512Async(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsBoolean()) {
        Napi::TypeError::New(env, "Tham số không hợp lệ. Cần truyền (String input, Boolean is_file)").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string input = info[0].As<Napi::String>().Utf8Value();
    bool is_file = info[1].As<Napi::Boolean>().Value();
    
    HashWorker* worker = new HashWorker(env, input, is_file);
    worker->Queue(); 
    return worker->GetPromise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "hash"), Napi::Function::New(env, HashSHA512Async));
    return exports;
}

NODE_API_MODULE(sha512_addon, Init)