use super::{Device, DeviceError};

pub struct Stdout {}

impl Stdout {
    pub fn new() -> Self {
        Self {}
    }
}

impl Device for Stdout {
    fn write(&mut self, addr: u32, data: &[u8]) -> Result<(), DeviceError> {
        for byte in data {
            self.write8(addr, *byte)?;
        }
        Ok(())
    }

    fn write8(&mut self, _addr: u32, value: u8) -> Result<(), DeviceError> {
        match std::char::from_u32(value as u32) {
            Some(c) => {
                print!("{}", c);
                Ok(())
            }
            None => Err(DeviceError::IllegalMemoryAccess),
        }
    }

    fn write32(&mut self, _addr: u32, value: u32) -> Result<(), DeviceError> {
        match std::char::from_u32(value) {
            Some(c) => {
                print!("{}", c);
                Ok(())
            }
            None => Err(DeviceError::IllegalMemoryAccess),
        }
    }

    fn read(&self, _addr: u32, _len: u32) -> Result<Vec<u8>, DeviceError> {
        Err(DeviceError::IllegalMemoryAccess)
    }
    fn read8(&self, _addr: u32) -> Result<u8, DeviceError> {
        Err(DeviceError::IllegalMemoryAccess)
    }
    fn read32(&self, _addr: u32) -> Result<u32, DeviceError> {
        Err(DeviceError::IllegalMemoryAccess)
    }
}
