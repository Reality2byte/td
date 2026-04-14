//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/CustomEmojiId.h"
#include "td/telegram/td_api.h"
#include "td/telegram/telegram_api.h"
#include "td/telegram/UserId.h"

#include "td/utils/common.h"

namespace td {

class Dependencies;
class Td;

class AiComposeTone {
  enum class Type : int32 { Default, Custom };
  Type type_ = Type::Default;
  string slug_;
  CustomEmojiId custom_emoji_id_;
  string title_;

  bool is_creator_ = false;
  int64 id_ = 0;
  int64 access_hash_ = 0;
  int32 install_count_ = 0;
  string prompt_;
  UserId author_user_id_;

  friend bool operator==(const AiComposeTone &lhs, const AiComposeTone &rhs);

 public:
  AiComposeTone() = default;

  explicit AiComposeTone(telegram_api::object_ptr<telegram_api::AiComposeTone> &&tone_ptr);

  td_api::object_ptr<td_api::textCompositionStyle> get_text_composition_style_object(Td *td) const;

  void add_dependencies(Dependencies &dependencies) const;

  template <class StorerT>
  void store(StorerT &storer) const;

  template <class ParserT>
  void parse(ParserT &parser);
};

bool operator==(const AiComposeTone &lhs, const AiComposeTone &rhs);

inline bool operator!=(const AiComposeTone &lhs, const AiComposeTone &rhs) {
  return !(lhs == rhs);
}

}  // namespace td
