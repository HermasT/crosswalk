// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_EXTENSIONS_BROWSER_XWALK_EXTENSION_SERVICE_H_
#define XWALK_EXTENSIONS_BROWSER_XWALK_EXTENSION_SERVICE_H_

#include <stdint.h>
#include <string>
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/synchronization/lock.h"
#include "xwalk/extensions/browser/xwalk_in_process_extension_handler.h"

namespace content {
class RenderProcessHost;
class WebContents;
}

namespace xwalk {
namespace extensions {

class XWalkExtension;
class XWalkExtensionServer;

// This is the entry point for Crosswalk extensions. Its responsible for keeping
// track of the extensions, and enable them on WebContents once they are
// created. It's life time follows the Browser process itself.
class XWalkExtensionService {
 public:
  explicit XWalkExtensionService();
  virtual ~XWalkExtensionService();

  // Returns false if it couldn't be registered because another one with the
  // same name exists, otherwise returns true.
  bool RegisterExtension(scoped_ptr<XWalkExtension> extension);

  void RegisterExternalExtensionsForPath(const base::FilePath& path);

  // To be called when a new RenderProcessHost is created, will plug the
  // extension system to that render process. See
  // XWalkContentBrowserClient::RenderProcessHostCreated().
  void OnRenderProcessHostCreated(content::RenderProcessHost* host);

  typedef base::Callback<void(XWalkExtensionService* extension_service)>
      RegisterExtensionsCallback;
  static void SetRegisterExtensionsCallbackForTesting(
      const RegisterExtensionsCallback& callback);

 private:
  void RegisterExtensionsForNewHost(content::RenderProcessHost* host);

  base::Lock in_process_extensions_lock_;
  XWalkInProcessExtensionHandler in_process_extensions_;

  // FIXME(cmarcelo): For now we support only one render process host.
  content::RenderProcessHost* render_process_host_;

  scoped_ptr<XWalkExtensionServer> in_process_extensions_server_;

  DISALLOW_COPY_AND_ASSIGN(XWalkExtensionService);
};

bool ValidateExtensionNameForTesting(const std::string& extension_name);

}  // namespace extensions
}  // namespace xwalk

#endif  // XWALK_EXTENSIONS_BROWSER_XWALK_EXTENSION_SERVICE_H_
