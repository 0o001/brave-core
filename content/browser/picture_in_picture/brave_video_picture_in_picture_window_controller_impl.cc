#include "brave/content/browser/picture_in_picture/brave_video_picture_in_picture_window_controller_impl.h"

#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"


namespace content {

// static
BraveVideoPictureInPictureWindowControllerImpl*
BraveVideoPictureInPictureWindowControllerImpl::GetOrCreateForWebContents(
    WebContents* web_contents) {
  DCHECK(web_contents);

  // This is a no-op if the controller already exists.
  CreateForWebContents(web_contents);
  return FromWebContents(web_contents);
}

BraveVideoPictureInPictureWindowControllerImpl::
    ~BraveVideoPictureInPictureWindowControllerImpl() = default;

BraveVideoPictureInPictureWindowControllerImpl::
    BraveVideoPictureInPictureWindowControllerImpl(WebContents* web_contents)
    : VideoPictureInPictureWindowControllerImpl(web_contents),
      WebContentsUserData<BraveVideoPictureInPictureWindowControllerImpl>(
          *web_contents) {}

}  // namespace content
