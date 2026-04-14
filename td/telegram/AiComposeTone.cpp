//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "td/telegram/AiComposeTone.h"

#include "td/telegram/Dependencies.h"
#include "td/telegram/Td.h"
#include "td/telegram/UserManager.h"

#include "td/utils/logging.h"

namespace td {

AiComposeTone::AiComposeTone(telegram_api::object_ptr<telegram_api::AiComposeTone> &&tone_ptr) {
  CHECK(tone_ptr != nullptr);
  switch (tone_ptr->get_id()) {
    case telegram_api::aiComposeTone::ID: {
      auto tone = telegram_api::move_object_as<telegram_api::aiComposeTone>(tone_ptr);
      type_ = Type::Custom;
      slug_ = std::move(tone->slug_);
      custom_emoji_id_ = CustomEmojiId(tone->emoji_id_);
      title_ = std::move(tone->title_);
      is_creator_ = tone->creator_;
      id_ = tone->id_;
      access_hash_ = tone->access_hash_;
      install_count_ = tone->installs_count_;
      prompt_ = std::move(tone->prompt_);
      author_user_id_ = UserId(tone->author_id_);
      break;
    }
    case telegram_api::aiComposeToneDefault::ID: {
      auto tone = telegram_api::move_object_as<telegram_api::aiComposeToneDefault>(tone_ptr);
      type_ = Type::Default;
      slug_ = std::move(tone->tone_);
      custom_emoji_id_ = CustomEmojiId(tone->emoji_id_);
      title_ = std::move(tone->title_);
      break;
    }
    default:
      UNREACHABLE();
  }
  if (author_user_id_ != UserId() && !author_user_id_.is_valid()) {
    LOG(ERROR) << "Receive " << author_user_id_;
    author_user_id_ = {};
  }
  if (custom_emoji_id_ != CustomEmojiId() && !custom_emoji_id_.is_valid()) {
    LOG(ERROR) << "Receive " << custom_emoji_id_;
    custom_emoji_id_ = {};
  }
}

td_api::object_ptr<td_api::textCompositionStyle> AiComposeTone::get_text_composition_style_object(Td *td) const {
  return td_api::make_object<td_api::textCompositionStyle>(
      slug_, custom_emoji_id_.get(), title_, type_ == Type::Custom, is_creator_, install_count_, prompt_,
      td->user_manager_->get_user_id_object(author_user_id_, "textCompositionStyle"));
}

void AiComposeTone::add_dependencies(Dependencies &dependencies) const {
  dependencies.add(author_user_id_);
}

bool operator==(const AiComposeTone &lhs, const AiComposeTone &rhs) {
  return lhs.type_ == rhs.type_ && lhs.slug_ == rhs.slug_ && lhs.custom_emoji_id_ == rhs.custom_emoji_id_ &&
         lhs.title_ == rhs.title_ && lhs.is_creator_ == rhs.is_creator_ && lhs.id_ == rhs.id_ &&
         lhs.access_hash_ == rhs.access_hash_ && lhs.install_count_ == rhs.install_count_ &&
         lhs.prompt_ == rhs.prompt_ && lhs.author_user_id_ == rhs.author_user_id_;
}

}  // namespace td
