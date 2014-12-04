////////////////////////////////////////////////////////////////////////////////
//
//  Designed by Matthew Lee, 12/3/2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ios_push_service__
#define __ios_push_service__

#include <capn/apn.h>
#include <string>
#include <stdexcept>

namespace apn {

////////////////////////////////////////////////////////////////////////////////

class PushAction {
 public:
  PushAction(apn_ctx_ref ctx, std::string const& token, std::string const& text,
             int badge, std::string const& sound, int exp_secs = 60 * 60);
  ~PushAction();
  void close();

  std::string const& token() const { return _token; }
  std::string const& text() const { return _text; }
  int badge() const { return _badge; }
  std::string const& sound() const { return _sound; }
  int exp_secs() const { return _exp_secs; }

  apn_payload_ctx_ref payload_ctx() { return _payload_ctx; }

 private:
  apn_payload_ctx_ref _payload_ctx;
  apn_error_ref _last_error;
  std::string _token;
  std::string _text;
  int _badge;
  std::string _sound;
  int _exp_secs;
};

////////////////////////////////////////////////////////////////////////////////

class PushManager {
 public:
  PushManager(std::string const& cert_path, std::string const& key_path);
  ~PushManager();
  void close();

  void send(PushAction* action);
  PushAction* create_action(std::string const& token, std::string const& text,
                            int badget, std::string const& sound,
                            int exp_secs = 60 * 60);

 private:
  apn_ctx_ref _ctx;
  apn_error_ref _last_error;
};

}  // namespace apn

#endif /* defined(__ios_push_service__) */
