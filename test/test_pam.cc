#include <security/pam_appl.h>
#include <gtest/gtest.h>

TEST(pam, start)
{
    pam_handle_t * handle = nullptr;
    struct pam_conv conv = { nullptr, nullptr };
    int rc = pam_start("test", nullptr, &conv, &handle);
    if (PAM_SUCCESS == rc)
    {
        pam_end(handle, 0);
    }
}