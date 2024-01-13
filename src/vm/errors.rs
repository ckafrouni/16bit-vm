use super::{cpu::CPUError, devices::DeviceError, mmu::MMUError};

#[derive(Debug, Clone, Copy)]
pub enum VMError {
    CPUError(CPUError),
    MMUError(MMUError),
    DeviceError(DeviceError),
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

impl From<DeviceError> for VMError {
    fn from(err: DeviceError) -> Self {
        VMError::DeviceError(err)
    }
}
