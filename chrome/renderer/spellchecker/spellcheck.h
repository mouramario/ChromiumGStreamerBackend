// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_H_
#define CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_H_

#include <set>
#include <string>
#include <vector>

#include "base/files/file.h"
#include "base/gtest_prod_util.h"
#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/renderer/spellchecker/custom_dictionary_engine.h"
#include "content/public/renderer/render_process_observer.h"

struct SpellCheckBDictLanguage;
class SpellcheckLanguage;
struct SpellCheckResult;

namespace blink {
class WebTextCheckingCompletion;
struct WebTextCheckingResult;
template <typename T> class WebVector;
}

namespace IPC {
class Message;
}

// TODO(morrita): Needs reorg with SpellCheckProvider.
// See http://crbug.com/73699.
// Shared spellchecking logic/data for a RenderProcess. All RenderViews use
// this object to perform spellchecking tasks.
class SpellCheck : public content::RenderProcessObserver,
                   public base::SupportsWeakPtr<SpellCheck> {
 public:
  // TODO(groby): I wonder if this can be private, non-mac only.
  class SpellcheckRequest;
  enum ResultFilter {
    DO_NOT_MODIFY = 1,  // Do not modify results.
    USE_NATIVE_CHECKER,  // Use native checker to double-check.
  };

  SpellCheck();
  ~SpellCheck() override;

  void AddSpellcheckLanguage(base::File file, const std::string& language);

  // If there is no dictionary file, then this requests one from the browser
  // and does not block. In this case it returns true.
  // If there is a dictionary file, but Hunspell has not been loaded, then
  // this loads Hunspell.
  // If Hunspell is already loaded, this does nothing. In both the latter cases
  // it returns false, meaning that it is OK to continue spellchecking.
  bool InitializeIfNeeded();

  // SpellCheck a word.
  // Returns true if spelled correctly, false otherwise.
  // If the spellchecker failed to initialize, always returns true.
  // The |tag| parameter should either be a unique identifier for the document
  // that the word came from (if the current platform requires it), or 0.
  // In addition, finds the suggested words for a given word
  // and puts them into |*optional_suggestions|.
  // If the word is spelled correctly, the vector is empty.
  // If optional_suggestions is NULL, suggested words will not be looked up.
  // Note that Doing suggest lookups can be slow.
  bool SpellCheckWord(const base::char16* in_word,
                      int in_word_len,
                      int tag,
                      int* misspelling_start,
                      int* misspelling_len,
                      std::vector<base::string16>* optional_suggestions);

  // SpellCheck a paragraph.
  // Returns true if |text| is correctly spelled, false otherwise.
  // If the spellchecker failed to initialize, always returns true.
  bool SpellCheckParagraph(
      const base::string16& text,
      blink::WebVector<blink::WebTextCheckingResult>* results);

  // Find a possible correctly spelled word for a misspelled word. Computes an
  // empty string if input misspelled word is too long, there is ambiguity, or
  // the correct spelling cannot be determined.
  // NOTE: If using the platform spellchecker, this will send a *lot* of sync
  // IPCs. We should probably refactor this if we ever plan to take it out from
  // behind its command line flag.
  base::string16 GetAutoCorrectionWord(const base::string16& word, int tag);

  // Requests to spellcheck the specified text in the background. This function
  // posts a background task and calls SpellCheckParagraph() in the task.
#if !defined (OS_MACOSX)
  void RequestTextChecking(const base::string16& text,
                           blink::WebTextCheckingCompletion* completion);
#endif

  // Creates a list of WebTextCheckingResult objects (used by WebKit) from a
  // list of SpellCheckResult objects (used by Chrome). This function also
  // checks misspelled words returned by the Spelling service and changes the
  // underline colors of contextually-misspelled words.
  void CreateTextCheckingResults(
      ResultFilter filter,
      int line_offset,
      const base::string16& line_text,
      const std::vector<SpellCheckResult>& spellcheck_results,
      blink::WebVector<blink::WebTextCheckingResult>* textcheck_results);

  bool is_spellcheck_enabled() { return spellcheck_enabled_; }

 private:
   friend class SpellCheckTest;
   FRIEND_TEST_ALL_PREFIXES(SpellCheckTest, GetAutoCorrectionWord_EN_US);
   FRIEND_TEST_ALL_PREFIXES(SpellCheckTest,
       RequestSpellCheckMultipleTimesWithoutInitialization);

  // RenderProcessObserver implementation:
   bool OnControlMessageReceived(const IPC::Message& message) override;

  // Message handlers.
   void OnInit(const std::vector<SpellCheckBDictLanguage>& bdict_languages,
               const std::set<std::string>& custom_words,
               bool auto_spell_correct);
   void OnCustomDictionaryChanged(const std::set<std::string>& words_added,
                                  const std::set<std::string>& words_removed);
  void OnEnableAutoSpellCorrect(bool enable);
  void OnEnableSpellCheck(bool enable);
  void OnRequestDocumentMarkers();

#if !defined (OS_MACOSX)
  // Posts delayed spellcheck task and clear it if any.
  // Takes ownership of |request|.
  void PostDelayedSpellCheckTask(SpellcheckRequest* request);

  // Performs spell checking from the request queue.
  void PerformSpellCheck(SpellcheckRequest* request);

  // The parameters of a pending background-spellchecking request. When WebKit
  // sends a background-spellchecking request before initializing hunspell,
  // we save its parameters and start spellchecking after we finish initializing
  // hunspell. (When WebKit sends two or more requests, we cancel the previous
  // requests so we do not have to use vectors.)
  scoped_ptr<SpellcheckRequest> pending_request_param_;
#endif

  // A vector of objects used to actually check spelling, one for each enabled
  // language.
  ScopedVector<SpellcheckLanguage> languages_;

  // Custom dictionary spelling engine.
  CustomDictionaryEngine custom_dictionary_;

  // Remember state for auto spell correct.
  bool auto_spell_correct_turned_on_;

  // Remember state for spellchecking.
  bool spellcheck_enabled_;

  DISALLOW_COPY_AND_ASSIGN(SpellCheck);
};

#endif  // CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_H_
