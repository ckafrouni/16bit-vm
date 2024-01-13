use crate::vm::DeviceError;

#[derive(Debug, Clone, Copy)]
pub enum MMUError {
    InvalidAddress,
    IllegalMemoryAccess,
    DeviceError(DeviceError),
}

impl From<DeviceError> for MMUError {
    fn from(err: DeviceError) -> Self {
        MMUError::DeviceError(err)
    }
}
