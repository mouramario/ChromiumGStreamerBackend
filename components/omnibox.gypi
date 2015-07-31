# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      # GN version: //components/omnibox/browser
      'target_name': 'omnibox_browser',
      'type': 'static_library',
      'dependencies': [
        '../base/base.gyp:base',
        '../base/base.gyp:base_i18n',
        '../net/net.gyp:net',
        '../sql/sql.gyp:sql',
        '../third_party/protobuf/protobuf.gyp:protobuf_lite',
        '../ui/base/ui_base.gyp:ui_base',
        '../url/url.gyp:url_lib',
        'bookmarks_browser',
        'component_metrics_proto',
        'components_resources.gyp:components_resources',
        'components_strings.gyp:components_strings',
        'history_core_browser',
        'keyed_service_core',
        'omnibox_common',
        'omnibox_in_memory_url_index_cache_proto',
        'pref_registry',
        'query_parser',
        'search',
        'search_engines',
        'url_fixer',
        'variations_http_provider',
      ],
      'export_dependent_settings': [
        'component_metrics_proto',
        'history_core_browser',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        # Note: sources list duplicated in GN build.
        'omnibox/browser/answers_cache.cc',
        'omnibox/browser/answers_cache.h',
        'omnibox/browser/autocomplete_classifier.cc',
        'omnibox/browser/autocomplete_classifier.h',
        'omnibox/browser/autocomplete_controller.cc',
        'omnibox/browser/autocomplete_controller.h',
        'omnibox/browser/autocomplete_controller_delegate.h',
        'omnibox/browser/autocomplete_input.cc',
        'omnibox/browser/autocomplete_input.h',
        'omnibox/browser/autocomplete_match.cc',
        'omnibox/browser/autocomplete_match.h',
        'omnibox/browser/autocomplete_match_type.cc',
        'omnibox/browser/autocomplete_match_type.h',
        'omnibox/browser/autocomplete_provider.cc',
        'omnibox/browser/autocomplete_provider.h',
        'omnibox/browser/autocomplete_provider_client.h',
        'omnibox/browser/autocomplete_provider_listener.h',
        'omnibox/browser/autocomplete_result.cc',
        'omnibox/browser/autocomplete_result.h',
        'omnibox/browser/autocomplete_scheme_classifier.h',
        'omnibox/browser/base_search_provider.cc',
        'omnibox/browser/base_search_provider.h',
        'omnibox/browser/bookmark_provider.cc',
        'omnibox/browser/bookmark_provider.h',
        'omnibox/browser/builtin_provider.cc',
        'omnibox/browser/builtin_provider.h',
        'omnibox/browser/history_provider.cc',
        'omnibox/browser/history_provider.h',
        'omnibox/browser/history_quick_provider.cc',
        'omnibox/browser/history_quick_provider.h',
        'omnibox/browser/history_url_provider.cc',
        'omnibox/browser/history_url_provider.h',
        'omnibox/browser/in_memory_url_index.cc',
        'omnibox/browser/in_memory_url_index.h',
        'omnibox/browser/in_memory_url_index_types.cc',
        'omnibox/browser/in_memory_url_index_types.h',
        'omnibox/browser/keyword_extensions_delegate.cc',
        'omnibox/browser/keyword_extensions_delegate.h',
        'omnibox/browser/keyword_provider.cc',
        'omnibox/browser/keyword_provider.h',
        'omnibox/browser/omnibox_client.h',
        'omnibox/browser/omnibox_field_trial.cc',
        'omnibox/browser/omnibox_field_trial.h',
        'omnibox/browser/omnibox_log.cc',
        'omnibox/browser/omnibox_log.h',
        'omnibox/browser/omnibox_popup_model_observer.h',
        'omnibox/browser/omnibox_popup_view.h',
        'omnibox/browser/omnibox_pref_names.cc',
        'omnibox/browser/omnibox_pref_names.h',
        'omnibox/browser/omnibox_switches.cc',
        'omnibox/browser/omnibox_switches.h',
        'omnibox/browser/scored_history_match.cc',
        'omnibox/browser/scored_history_match.h',
        'omnibox/browser/search_provider.cc',
        'omnibox/browser/search_provider.h',
        'omnibox/browser/search_suggestion_parser.cc',
        'omnibox/browser/search_suggestion_parser.h',
        'omnibox/browser/shortcuts_backend.cc',
        'omnibox/browser/shortcuts_backend.h',
        'omnibox/browser/shortcuts_database.cc',
        'omnibox/browser/shortcuts_database.h',
        'omnibox/browser/shortcuts_provider.cc',
        'omnibox/browser/shortcuts_provider.h',
        'omnibox/browser/suggestion_answer.cc',
        'omnibox/browser/suggestion_answer.h',
        'omnibox/browser/url_index_private_data.cc',
        'omnibox/browser/url_index_private_data.h',
        'omnibox/browser/url_prefix.cc',
        'omnibox/browser/url_prefix.h',
        'omnibox/browser/zero_suggest_provider.cc',
        'omnibox/browser/zero_suggest_provider.h',
      ],
    },
    {
      # GN version: //components/omnibox/common
      'target_name': 'omnibox_common',
      'type': 'none',
      'include_dirs': [
        '..',
      ],
      'sources': [
        # Note: sources list duplicated in GN build.
        'omnibox/common/omnibox_focus_state.h',
      ],
    },
    {
      # Protobuf compiler / generator for the InMemoryURLIndex caching
      # protocol buffer.
      # GN version: //components/omnibox:in_memory_url_index_cache_proto
      'target_name': 'omnibox_in_memory_url_index_cache_proto',
      'type': 'static_library',
      'sources': [ 'omnibox/browser/in_memory_url_index_cache.proto', ],
      'variables': {
        'proto_in_dir': 'omnibox/browser',
        'proto_out_dir': 'components/omnibox/browser',
      },
      'includes': [ '../build/protoc.gypi', ],
    },
    {
      # GN version: //components/omnibox:test_support
      'target_name': 'omnibox_test_support',
      'type': 'static_library',
      'dependencies': [
        '../base/base.gyp:base',
        '../testing/gmock.gyp:gmock',
        'omnibox_browser',
        'component_metrics_proto',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        # Note: sources list duplicated in GN build.
        'omnibox/browser/mock_autocomplete_provider_client.cc',
        'omnibox/browser/mock_autocomplete_provider_client.h',
        'omnibox/browser/test_scheme_classifier.cc',
        'omnibox/browser/test_scheme_classifier.h',
      ],
    },
  ],
}
