#include <napi.h>
#include "utils/padding_parsing.h"
#include "sha2/sha512_core.h"

// Hàm wrapper: Được gọi từ Javascript
Napi::String HashSHA512Wrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Kiểm tra xem Javascript có truyền đúng 1 chuỗi (string) xuống không
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a string as the first argument").ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    // Lấy chuỗi từ JS và ép kiểu về UTF-8 chuẩn
    std::string input_text = info[0].As<Napi::String>().Utf8Value();

    // Gọi lõi C++ của bạn
    std::vector<uint8_t> padded_data = pad_and_parse(input_text);
    std::string hash_result = sha512::hash_padded_data(padded_data);

    // Trả kết quả về lại cho Javascript
    return Napi::String::New(env, hash_result);
}

// Khai báo module cho Node.js biết
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Expose hàm HashSHA512Wrapped ra thành hàm "hash" trong Javascript
    exports.Set(Napi::String::New(env, "hash"), Napi::Function::New(env, HashSHA512Wrapped));
    return exports;
}

NODE_API_MODULE(sha512_addon, Init)