// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_UI_IME_IME_REGISTRAR_IMPL_H_
#define SERVICES_UI_IME_IME_REGISTRAR_IMPL_H_

#include "mojo/public/cpp/bindings/binding_set.h"
#include "services/ui/ime/ime_server_impl.h"
#include "services/ui/public/interfaces/ime.mojom.h"

namespace ui {

class IMERegistrarImpl : public mojom::IMERegistrar {
 public:
  explicit IMERegistrarImpl(IMEServerImpl* ime_server);
  ~IMERegistrarImpl() override;

  void AddBinding(mojom::IMERegistrarRequest request);

 private:
  // mojom::IMERegistrar:
  void RegisterDriver(mojom::IMEDriverPtr driver) override;

  mojo::BindingSet<mojom::IMERegistrar> bindings_;
  IMEServerImpl* ime_server_;

  DISALLOW_COPY_AND_ASSIGN(IMERegistrarImpl);
};

}  // namespace ui

#endif  // SERVICES_UI_IME_IME_REGISTRAR_IMPL_H_
