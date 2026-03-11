# Security policy for Orbit

## Reporting issues

If you experiment with Orbit and notice a security problem in the code or build setup:

- Open an issue in the repository describing:
  - The affected component (kernel, driver, GUI, build)
  - Steps to reproduce
  - Expected vs actual behavior

There is no formal SLA for responses; reports are handled on a best‑effort basis.

## Guiding Principles

Orbit is not a hobby or experimental operating system — it is engineered with professional, robust, and large-scale ambitions. Security is a first-class concern, and contributions to Orbit are expected to meet high standards for safe and resilient system design.

- Treat every component and interaction (kernel, driver, GUI, build, libraries) as potentially security-sensitive.
- Proactively design with defense-in-depth and least privilege in mind.
- Review all external dependencies for potential risks. Avoid unnecessary dependencies.
- Document security-relevant behaviors and potential attack surfaces in code and commit messages.

## Coordinated Disclosure

If you discover a vulnerability in Orbit:

- File a private GitHub security advisory **or** email the core team via security@orbit.org
- Clearly describe:
  - The Orbit version/commit
  - Impact and severity
  - Steps to reproduce or proof-of-concept
  - Any ideas for possible mitigation

We will coordinate to assess and address the issue prior to public disclosure.

## Security Maintenance

- All critical security fixes are prioritized and tracked publicly once patched.
- Binary artifacts are not stored in the repository; all releases are built from source in clean environments.
- Automated and manual review processes are evolving to meet enterprise-grade expectations.

## Commitment

Orbit is dedicated to providing a secure foundation for applications at scale: in education, research, and real-world deployments. Thank you for helping us deliver an OS that meets these ambitions.
