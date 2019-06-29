

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::lock_guard;
using std::move;
using std::mutex;
using std::setprecision;
using std::string;
using std::thread;
using std::to_string;
using std::unique_lock;
using std::vector;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::this_thread::sleep_for;

class ServiceRequest {
 public:
  ServiceRequest(const string& s) : request_(s) {}

  string ExtractWordToCheckFromRequest() { return request_; }

 private:
  string request_;
};

class ServiceResponse {
 public:
  const vector<string>& response() { return response_; }

  void EncodeIntoResponse(const vector<string>& s) { response_ = s; }

 private:
  vector<string> response_;
};

vector<string> ClosestInDictionary(const string& w) {
  sleep_for(milliseconds(200));
  return {w + "_result"};
}

class SpellCheckService {
 public:
  static void Service(ServiceRequest& req, ServiceResponse& resp) {
    if (string w = req.ExtractWordToCheckFromRequest(); w != w_last_) {
      w_last_ = move(w);
      closest_to_last_word_ = ClosestInDictionary(w_last_);
    }
    resp.EncodeIntoResponse(closest_to_last_word_);
  }

 private:
  static string w_last_;
  static vector<string> closest_to_last_word_;
};

string SpellCheckService::w_last_;
vector<string> SpellCheckService::closest_to_last_word_;

void ServiceThread(const string& data) {
  static mutex mx;
  auto start_time = system_clock::now();
  ServiceRequest req(data);
  ServiceResponse resp;
  lock_guard<mutex> lock(mx);
  SpellCheckService::Service(req, resp);
  duration<float> running_time = system_clock::now() - start_time;
  cout << data << " -> " << resp.response()[0] << " (" << setprecision(3)
       << running_time.count() << " sec)" << endl;
}

int main(int argc, char* argv[]) {
  int i = 0;
  while (true) {
    thread(ServiceThread, "req:" + to_string(i + 1)).detach();
    if (i > 0) {
      // while req:i+1 is computed we could return req:i from the cache
      thread(ServiceThread, "req:" + to_string(i)).detach();
    }
    sleep_for(milliseconds(500));
    ++i;
  }
}
