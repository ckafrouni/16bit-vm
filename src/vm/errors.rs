use super::{cpu::CPUError, mmu::MMUError};

#[derive(Debug, Clone)]
pub enum VMError {
    CPUError(CPUError),
    MMUError(MMUError),
}

impl From<CPUError> for VMError {
    fn from(err: CPUError) -> Self {
        VMError::CPUError(err)
    }
}

impl From<MMUError> for VMError {
    fn from(err: MMUError) -> Self {
        VMError::MMUError(err)
    }
}
