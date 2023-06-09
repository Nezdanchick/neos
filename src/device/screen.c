#include <uefi.h>
#include "device/screen.h"

uintn_t isiz = sizeof(efi_gop_mode_info_t);
efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
efi_gop_t *gop = NULL;
uintn_t currentMode = 0;
efi_gop_mode_info_t *gopInfo;

void clear_screen() {
    ST->ConOut->ClearScreen(ST->ConOut);
}
static inline void set_pixel(int x, int y, uint32_t pixel)
{
   *((uint32_t*)
   (gop->Mode->FrameBufferBase + 4 * gopInfo->PixelsPerScanLine * y + 4 * x)) = pixel;
}
void set_videomode(int mode) {
    efi_status_t status = gop->SetMode(gop, mode);
    if(EFI_ERROR(status))
        fprintf(stderr, "Unable to set mode %03d", mode);
    gopInfo = gop->Mode->Information;
    currentMode = mode;
    ST->ConOut->Reset(ST->ConOut, 0);
}
void init_videomode() {
    efi_status_t status = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    status = gop->QueryMode(gop, gop->Mode->Mode, &isiz, &gopInfo);
    if (gop->Mode || EFI_ERROR(status)) {
        fprintf(stderr, "GOP init failue");
        set_videomode(0);
    }
}
void getinfo_framebuffer() {
    printf("framebuffer:\n-address = %x\n-size = %d\n-resolution = %dx%d\n",
      gop->Mode->FrameBufferBase,
      gop->Mode->FrameBufferSize,
      gopInfo->HorizontalResolution,
      (gop->Mode->FrameBufferSize/gopInfo->HorizontalResolution/4)
      // in qemu VerticalResolution equal to INT32_MAX
    );
}
void getinfo_videomodes(int collumns) {
    if (collumns <= 0) {
        fprintf(stderr, "getinfo_videomodes collumns must be greater than 0");
        return;
    }
    efi_status_t status;
    efi_gop_mode_info_t *info = gop->Mode->Information;
    printf("video modes:\n");
    if(!EFI_ERROR(status) && gop) {
        for(int i = 0; i < gop->Mode->MaxMode; i++) {
            status = gop->QueryMode(gop, i, &isiz, &info);
            if(EFI_ERROR(status) || info->PixelFormat > PixelBitMask) continue;

            printf(" %c%2d)%4d x %4d ",
                i == currentMode ? '*' : ' ', i,
                info->HorizontalResolution, info->VerticalResolution);
            if (i % collumns == collumns - 1) printf("\n");
        }
    } else
        fprintf(stderr, "unable to get graphics output protocol\n");
    printf("\n");
}