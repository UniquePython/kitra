# Security Policy

## Supported Versions

Only the latest release of Kitra receives security fixes. If you are using an older version, please upgrade before reporting.

## Reporting a Vulnerability

If you discover a security vulnerability in Kitra, please **do not open a public issue**. Instead, use GitHub's private disclosure feature:

👉 [Report a vulnerability](https://github.com/UniquePython/kitra/security/advisories/new)

Please include as much of the following as you can:
- A description of the vulnerability and its potential impact
- Steps to reproduce, or a minimal code sample that triggers the issue
- The version of Kitra you are using
- Your platform (Linux / macOS) and compiler version

## What to Expect

Since Kitra is maintained by a single developer, I'll do my best to:
- **Acknowledge** your report within a few days
- **Investigate and patch** the issue as quickly as I can
- **Credit you** in the release notes, unless you prefer to stay anonymous

## Scope

Kitra is a 2D graphics library, not a networked application or server. That said, valid security concerns include:

- A crafted image file that causes memory corruption when loaded via the texture API
- A crafted audio file that causes unsafe behavior via the audio API
- A crafted font file that causes unsafe behavior via the text API
- Any input that triggers undefined behavior, buffer overflows, or arbitrary code execution

The following are **out of scope**:
- Bugs in SDL2 or its extensions — please report those to the [SDL project](https://github.com/libsdl-org/SDL) directly
- Crashes caused by incorrect API usage (these are bugs, not security issues — open a regular issue)

## Disclosure Policy

Once a fix is available and a patched release is published, you are welcome to publicly disclose the vulnerability. If you'd like to coordinate timing, mention that in your report and we can work it out.
