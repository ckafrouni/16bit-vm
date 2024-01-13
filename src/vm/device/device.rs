#![allow(dead_code)]
#[derive(Debug)]
pub enum DeviceError {
    InvalidAddress,
    IllegalMemoryAccess,
    DeviceError,
}

pub trait Device {
    fn read8(&self, addr: u32) -> Result<u8, DeviceError>;
    fn read32(&self, addr: u32) -> Result<u32, DeviceError>;

    fn write8(&mut self, addr: u32, value: u8) -> Result<(), DeviceError>;
    fn write32(&mut self, addr: u32, value: u32) -> Result<(), DeviceError>;

    fn read(&self, addr: u32, len: u32) -> Result<Vec<u8>, DeviceError> {
        let mut data = Vec::new();
        for i in 0..len {
            data.push(self.read8(addr + i)?);
        }
        Ok(data)
    }

    fn write(&mut self, addr: u32, data: &[u8]) -> Result<(), DeviceError> {
        let mut offset = 0;
        for byte in data {
            self.write8(addr + offset, *byte)?;
            offset += 1;
        }
        Ok(())
    }

    fn slice(&self, start: u32, end: u32) -> Result<Vec<u8>, DeviceError> {
        self.read(start, end - start)
    }
}

// TODO: implement this
// pub trait SliceableDevice: Box<dyn Device>evice {
//     fn slice(&self, start: u32, end: u32) -> Self;
// }
