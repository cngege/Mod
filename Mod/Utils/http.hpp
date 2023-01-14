#pragma once
//
// *一个简单的针对UWP程序的 http/https 请求实现
// *CNGEGE
// *by https://learn.microsoft.com/en-us/windows/uwp/networking/httpclient?source=recommendations
//

//#include <Windows.h>
#include <string>
#include <ppltasks.h>
#include <fstream>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <io.h>

using namespace concurrency;
class NetClient {
public:
	int StatusCode = 200;
	winrt::Windows::Web::Http::HttpClient httpClient;
	winrt::Windows::Web::Http::HttpResponseMessage httpResponseMessage;

public:
	bool StatusSuccess() {
		if (httpResponseMessage) {
			return httpResponseMessage.IsSuccessStatusCode();
		}else {
			return false;
		}
	}

	std::string GetAsyncAsString() {
		try {
			httpResponseMessage.EnsureSuccessStatusCode();
			auto ret = winrt::to_string(httpResponseMessage.Content().ReadAsStringAsync().get());
			httpClient.Close();
			return ret;
		}
		catch (...) {
			return {};
		}
	}

	winrt::Windows::Storage::Streams::IBuffer GetAsyncAsBuffer() {
		try {
			httpResponseMessage.EnsureSuccessStatusCode();
			auto readAsBufferAsync = httpResponseMessage.Content().ReadAsBufferAsync();
			//create_task(readAsBufferAsync).wait();
			auto m_response = readAsBufferAsync.GetResults();
			//create_task(m_response).wait();
			httpClient.Close();
			return m_response;
		}
		catch (...) {
			return {};
		}
	}

	/// <summary>
	/// 将IBuffer流写入文件
	/// </summary>
	/// <param name="file">必须要在 [Microsoft.MinecraftUWP_8wekyb3d8bbwe] 文件夹下的绝对路径,否则崩溃</param>
	/// <param name="buff">IBuffer流</param>
	/// <returns>是否写入成功</returns>
	static bool WriteFile(std::string file, winrt::Windows::Storage::Streams::IBuffer& buff) {
		if (!buff) {
			return false;
		}
		try {
			std::replace(file.begin(), file.end(), '/', '\\');
			auto wstr = winrt::to_hstring(file);
			if (_access(file.c_str(),0)) {
				std::ofstream(file).close();
			}
			winrt::Windows::Storage::StorageFile sfile = winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(wstr).get();
			winrt::Windows::Storage::FileIO::WriteBufferAsync(sfile, buff);
			return true;
		}
		catch (...) {
			return false;
		}
	}

public:
	static NetClient GetAsync(std::string url) {
		NetClient network;
		winrt::Windows::Foundation::Uri requestUri{ winrt::to_hstring(url) };
		network.httpResponseMessage = network.httpClient.GetAsync(requestUri).get();
		network.StatusCode = (uint32_t)network.httpResponseMessage.StatusCode();
		return network;
	}

	static NetClient PostAsync(std::string url,std::string json = "{}") {
		NetClient network;
		winrt::Windows::Foundation::Uri requestUri{ winrt::to_hstring(url) };
		winrt::Windows::Web::Http::HttpStringContent jsonContent(
			//L"{ \"firstName\": \"Eliot\" }",
			winrt::to_hstring(json),
			winrt::Windows::Storage::Streams::UnicodeEncoding::Utf8,
			L"application/json");
		network.httpResponseMessage = network.httpClient.PostAsync(requestUri, jsonContent).get();
		network.StatusCode = (uint32_t)network.httpResponseMessage.StatusCode();
		return network;
	}
};
