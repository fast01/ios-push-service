//
//  ios_push_service.cpp
//  UMeServerChat
//
//  Created by Matthew Lee on 14/12/3.
//  Copyright (c) 2014年 QX. All rights reserved.
//

#include "ios_push_service.h"
#include "print_log.h"

#define CC(__ret__) __check(__ret__, _last_error)

namespace apn {

////////////////////////////////////////////////////////////////////////////////

void __check(uint8_t ret, apn_error_ref error) {
  if (ret == APN_ERROR) {
    // failed
    std::string err_msg = apn_error_message(error);
    PRINT_ERR("PushManager ERROR: %s", apn_error_message(error),
              APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
    apn_error_free(&error);
    throw std::runtime_error(err_msg.c_str());
  }
}

////////////////////////////////////////////////////////////////////////////////

PushAction::PushAction(apn_ctx_ref ctx, std::string const& token,
                       std::string const& text, int badge,
                       std::string const& sound, int exp_secs)
    : _payload_ctx(NULL), _last_error(NULL) {
  _token = token;
  _text = text;
  _badge = badge;
  _sound = sound;
  _exp_secs = exp_secs;
  try {
    CC(apn_payload_init(&_payload_ctx, &_last_error));
    CC(apn_payload_add_token(_payload_ctx, token.c_str(), NULL));
    CC(apn_payload_set_badge(_payload_ctx, badge, NULL));
    CC(apn_payload_set_body(_payload_ctx, _text.c_str(), NULL));
    CC(apn_payload_set_sound(_payload_ctx, _sound.c_str(), NULL));
    CC(apn_payload_set_expiry(_payload_ctx, (int)time(NULL) + exp_secs, NULL));
  } catch (std::exception& e) {
    close();
  }
}

PushAction::~PushAction() { close(); }

void PushAction::close() {
  if (_payload_ctx) {
    apn_payload_free(&_payload_ctx);
    _payload_ctx = NULL;
  }
  if (_last_error) {
    apn_error_free(&_last_error);
    _last_error = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////////

PushManager::PushManager(std::string const& cert_path,
                         std::string const& key_path)
    : _ctx(NULL), _last_error(NULL) {
  try {
    CC(apn_init(&_ctx, cert_path.c_str(), key_path.c_str(), NULL,
                &_last_error));
    apn_set_mode(_ctx, APN_MODE_SANDBOX, NULL);
  } catch (std::exception& e) {
    close();
  }
}

PushManager::~PushManager() { close(); }

void PushManager::close() {
  if (_ctx) {
    apn_free(&_ctx);
    _ctx = NULL;
  }
}

void PushManager::send(PushAction* action) {
  CC(apn_connect(_ctx, &_last_error));
  CC(apn_send(_ctx, action->payload_ctx(), &_last_error));
  apn_close(_ctx);
}

PushAction* PushManager::create_action(std::string const& token,
                                       std::string const& text, int badget,
                                       std::string const& sound, int exp_secs) {
  return new PushAction(_ctx, token, text, badget, sound, exp_secs);
}

}  // namespace apn